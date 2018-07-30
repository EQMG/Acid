cd ../

mkdir Build\Debug\bin\Resources
mkdir Build\Release\bin\Resources

mklink /J Build\Debug\bin\Resources\Engine Resources
mklink /J Build\Release\bin\Resources\Engine Resources
mklink /J Build\RelWithDebInfo\bin\Resources\Engine Resources

mkdir Build\Tests\TestDeferred\Resources
mkdir Build\Tests\TestGuis\Resources

mklink /J Build\Tests\TestDeferred\Resources\Engine Resources
mklink /J Build\Tests\TestGuis\Resources\Engine Resources