#SPDX-License-Identifier: MIT

from tqdm import tqdm
from git import Repo, RemoteProgress

import platform
import requests
import tempfile
import os
import shutil
import pathlib
import zipfile
import hashlib
import tarfile

script_folder = pathlib.Path(__file__).parent.resolve()

arm_gcc_folder = os.path.join(script_folder, "arm-gnu-toolchain-13.2")
STM32CubeG0_folder = os.path.join(script_folder, "STM32CubeG0")
openocd_folder = os.path.join(script_folder, "openocd")


class CloneProgress(RemoteProgress):

    def __init__(self):
        super().__init__()
        self.pbar = tqdm()

    def update(self, op_code, cur_count, max_count=None, message=""):
        self.pbar.total = max_count
        self.pbar.n = cur_count
        self.pbar.refresh()


def unpack_tarfile(filename, target_path):
    with tarfile.open(filename) as tf:
        print("Extracting...")
        for member in tqdm(tf.getmembers()):
            tf.extract(member, target_path)


def unpack_zipfile(filename, target_path):
    with zipfile.ZipFile(filename) as zf:
        print("Extracting...")
        for member in tqdm(zf.infolist()):
            zf.extract(member, target_path)


def sha256sum(filename):
    h = hashlib.sha256()
    b = bytearray(128 * 1024)
    mv = memoryview(b)
    with open(filename, "rb", buffering=0) as f:
        print("SHA256 Hashing...")
        with tqdm(
            total=os.path.getsize(filename), unit="B", unit_scale=True
        ) as progress_bar:
            while n := f.readinto(mv):
                progress_bar.update(len(b))
                h.update(mv[:n])
    return h.hexdigest()


def download_and_extract(
    url, sha256, destination_folder, compresed_file_format, strip=True
):
    response = requests.get(url, stream=True, allow_redirects=True)
    if response.status_code != 200:
        raise RuntimeError(
            f"Request to {url} returned status code {response.status_code}"
        )
    with tempfile.NamedTemporaryFile(delete=False) as file:
        total_size = int(response.headers.get("content-length", 0))
        block_size = 1024
        print(f"Downloading {url} {'Unknown size' if total_size==0 else ''}")
        with tqdm(total=total_size, unit="B", unit_scale=True) as progress_bar:
            for data in response.iter_content(block_size):
                progress_bar.update(len(data))
                file.write(data)

        file.flush()
        filename = file.name
        if sha256 != sha256sum(filename):
            raise RuntimeError(f"SHA256 verification failed for {url.rsplit('/', 1)}")

        with tempfile.TemporaryDirectory() as temp_dir:
            # shutil.unpack_archive(filename, temp_dir, compresed_file_format)
            if compresed_file_format == "tar":
                unpack_tarfile(filename, temp_dir)
            elif compresed_file_format == "zip":
                unpack_zipfile(filename, temp_dir)
            else:
                raise RuntimeError(f"Unsupported file format {compresed_file_format}")

            root_folder = (
                os.path.join(temp_dir, os.listdir(temp_dir)[0]) if strip else temp_dir
            )
            for item in os.listdir(root_folder):
                shutil.move(
                    os.path.join(root_folder, item),
                    os.path.join(destination_folder, item),
                )

def setup_stm32_cube_g0():
    print("STM32CubeG0 v1.6.2, setup")
    if not os.path.exists(STM32CubeG0_folder):
        Repo.clone_from(
            "https://github.com/STMicroelectronics/STM32CubeG0",
            STM32CubeG0_folder,
            branch="v1.6.2",
            progress=CloneProgress(),
        )

    else:
        repo = Repo(STM32CubeG0_folder)
        repo.git.checkout("v1.6.2")

    repo = Repo(STM32CubeG0_folder)
    repo.submodule_update(recursive=True, init=True)

    print("STM32CubeG0 v1.6.2, done")


def setup_arm_gcc():
    print("arm-gnu-toolchain-13.2, setup")
    platform_system = platform.system()
    platform_machine = platform.machine()
    urls = {
        (
            "Linux",
            "x86_64",
        ): "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz",
        (
            "Linux",
            "aarch64",
        ): "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-aarch64-arm-none-eabi.tar.xz",
        (
            "Windows",
            "AMD64",
        ): "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi.zip",
        (
            "Darwin",
            "x86_64",
        ): "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz",
        (
            "Darwin",
            "arm64",
        ): "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-arm64-arm-none-eabi.tar.xz",
    }
    sha = {
        (
            "Linux",
            "x86_64",
        ): "6cd1bbc1d9ae57312bcd169ae283153a9572bd6a8e4eeae2fedfbc33b115fdbb",
        (
            "Linux",
            "aarch64",
        ): "8fd8b4a0a8d44ab2e195ccfbeef42223dfb3ede29d80f14dcf2183c34b8d199a",
        (
            "Windows",
            "AMD64",
        ): "51d933f00578aa28016c5e3c84f94403274ea7915539f8e56c13e2196437d18f",
        (
            "Darwin",
            "x86_64",
        ): "075faa4f3e8eb45e59144858202351a28706f54a6ec17eedd88c9fb9412372cc",
        (
            "Darwin",
            "arm64",
        ): "57343df9a2d7c5c587b59ad011bf516ca286f6f9d0ef5957a3ad251f5579fd16",
    }
    compresed_file_format = {"Linux": "tar", "Windows": "zip", "Darwin": "tar"}

    url = urls.get((platform_system, platform_machine))
    sha = sha.get((platform_system, platform_machine))

    if url is None or sha is None:
        print(
            f"arm-gnu-toolchain-13.2 - Unsupported operating system or architecture: {platform_system}, {platform_machine}"
        )
        exit(0)

    if not os.path.exists(arm_gcc_folder):
        os.makedirs(arm_gcc_folder, exist_ok=True)
        download_and_extract(
            url, sha, arm_gcc_folder, compresed_file_format[platform_system]
        )

    print("arm-gnu-toolchain-13.2, Done")


def setup_openocd():
    print("openocd v0.12.0, setup")
    platform_system = platform.system()
    if platform_system == "Windows":
        url = "https://github.com/openocd-org/openocd/releases/download/v0.12.0/openocd-v0.12.0-i686-w64-mingw32.tar.gz"
        sha = "d7168545a6d5df4772b6090d470650f3eb8c9732dbd19b1f9027824c7f4a6fa3"
        if not os.path.exists(openocd_folder):
            os.makedirs(openocd_folder, exist_ok=True)
            download_and_extract(url, sha, openocd_folder, "tar", strip=False)
    else:
        print("Please use you native package manager to install openocd")
    print("openocd v0.12.0, done")


setup_stm32_cube_g0()
setup_arm_gcc()
setup_openocd()
