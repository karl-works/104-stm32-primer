#SPDX-License-Identifier: MIT 
 
Write-Host "Rebuilding projects..." 
Remove-Item -Path "build" -Recurse -ErrorAction Ignore 
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release 
cmake --build build  
 
if ($LASTEXITCODE -ne 0)  
{ 
    Write-Host "Build failed." -ForegroundColor Red 
} 
else 
{ 
    Write-Host "Build succeeded." -ForegroundColor Green 
} 
