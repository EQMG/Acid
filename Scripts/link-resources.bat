cd ../

mkdir Build\Debug\bin\Resources
mkdir Build\Release\bin\Resources

mklink /J Build\Debug\bin\Resources\Engine Resources
mklink /J Build\Release\bin\Resources\Engine Resources
mklink /J Build\RelWithDebInfo\bin\Resources\Engine Resources

mklink /J Build\Tests\TestPhysics\Resources\Engine Resources
mklink /J Build\Tests\TestGuis\Resources\Engine Resources