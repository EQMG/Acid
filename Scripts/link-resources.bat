cd ../

mkdir Build\Debug\bin\Resources

mklink /J Build\Debug\bin\Resources\Engine Resources
mklink /J Build\Release\bin\Resources\Engine Resources
mklink /J Build\RelWithDebInfo\bin\Resources\Engine Resources

mklink /J Build\Tests\TestDeferred\Resources\Engine Resources
mklink /J Build\Tests\TestGuis\Resources\Engine Resources