param (
    [Parameter(Mandatory=$true)]
    [string]$target
)

Write-Host "Deploying project ./build/$target/$target.elf..."
.\extern\openocd\bin\openocd.exe -f board/st_nucleo_g0.cfg -c "program ./build/$target/$target.elf verify reset exit" -d0
if ($LASTEXITCODE -ne 0) 
{
    Write-Host "Deployment failed." -ForegroundColor Red
}
else
{
    Write-Host "Deployment succeeded." -ForegroundColor Green
}