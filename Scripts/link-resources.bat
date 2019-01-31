cd ../

IF EXIST cmake-build-debug\bin (
mkdir cmake-build-debug\bin\Resources
mklink /J cmake-build-debug\bin\Resources\Engine Resources
)

IF EXIST cmake-build-release\bin (
mkdir cmake-build-release\bin\Resources
mklink /J cmake-build-release\bin\Resources\Engine Resources
)

IF EXIST Build\Debug\Bin (
mkdir Build\Debug\Bin\Resources
mklink /J Build\Debug\Bin\Resources\Engine Resources
)

IF EXIST Build\Debug\Bin32 (
mkdir Build\Debug\Bin32\Resources
mklink /J Build\Debug\Bin32\Resources\Engine Resources
)

IF EXIST Build\Release\Bin (
mkdir Build\Release\Bin\Resources
mklink /J Build\Release\Bin\Resources\Engine Resources
)

IF EXIST Build\Release\Bin32 (
mkdir Build\Release\Bin32\Resources
mklink /J Build\Release\Bin32\Resources\Engine Resources
)

IF EXIST Build\bin\Debug (
mkdir Build\bin\Debug\Resources
mklink /J Build\bin\Debug\Resources\Engine Resources
)

IF EXIST Build\bin\Release (
mkdir Build\bin\Release\Resources
mklink /J Build\bin\Release\Resources\Engine Resources
)

IF EXIST Build\Tests\TestFont (
mkdir Build\Tests\TestFont\Resources
mklink /J Build\Tests\TestFont\Resources\Engine Resources
)

IF EXIST Build\Tests\TestGUI (
mkdir Build\Tests\TestGUI\Resources
mklink /J Build\Tests\TestGUI\Resources\Engine Resources
)

IF EXIST Build\Tests\TestPBR (
mkdir Build\Tests\TestPBR\Resources
mklink /J Build\Tests\TestPBR\Resources\Engine Resources
)

IF EXIST Build\Tests\TestPhysics (
mkdir Build\Tests\TestPhysics\Resources
mklink /J Build\Tests\TestPhysics\Resources\Engine Resources
)

pause
