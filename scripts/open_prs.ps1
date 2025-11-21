param(
    [switch]$OpenAll
)

# Helper to open the feature branch PR pages in the default browser.
# Usage: .\scripts\open_prs.ps1   Or: .\scripts\open_prs.ps1 -OpenAll

$base = 'https://github.com/Chamoda-S/Parallel-Numerical-Integration'
$prs = @(
    "$base/pull/new/feature/openmp",
    "$base/pull/new/feature/mpi",
    "$base/pull/new/feature/cuda"
)

if ($OpenAll) {
    foreach ($u in $prs) { Start-Process $u }
    Write-Host "Opened all PR pages in your default browser." -ForegroundColor Green
    exit 0
}

Write-Host "PR helper - choose which PR to open:" -ForegroundColor Cyan
Write-Host "1) OpenMP PR - $($prs[0])"
Write-Host "2) MPI PR    - $($prs[1])"
Write-Host "3) CUDA PR   - $($prs[2])"
Write-Host "A) Open all"
Write-Host "Q) Quit"

while ($true) {
    $key = Read-Host "Enter selection (1/2/3/A/Q)"
    switch ($key.ToUpper()) {
        '1' { Start-Process $prs[0]; break }
        '2' { Start-Process $prs[1]; break }
        '3' { Start-Process $prs[2]; break }
        'A' { foreach ($u in $prs) { Start-Process $u }; break }
        'Q' { break }
        default { Write-Host "Invalid selection" -ForegroundColor Yellow }
    }
}

Write-Host "Done." -ForegroundColor Green
