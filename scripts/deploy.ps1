param ( 
    [Parameter(Mandatory=$true)] 
    [string]$target 
) 
 
Write-Host "Deploying project ./build/app/$target/$target.elf..." 
.\extern\openocd\bin\openocd.exe -f board/st_nucleo_g0.cfg -c "program ./build/app/$target/$target.elf verify reset exit" 
if ($LASTEXITCODE -ne 0)  
{ 
    Write-Host "Deployment failed." -ForegroundColor Red 
} 
else 
{ 
    Write-Host "Deployment succeeded." -ForegroundColor Green 
}