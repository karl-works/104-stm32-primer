Write-Host "Building projects..."
cmake --build build 
if ($LASTEXITCODE -ne 0) 
{
    Write-Host "Build failed." -ForegroundColor Red
}
else
{
    Write-Host "Build succeeded." -ForegroundColor Green
}