cd ../

mklink /J Build\Debug\bin\Resources Resources
mklink /J Build\Release\bin\Resources Resources
mklink /J Build\RelWithDebInfo\bin\Resources Resources

mklink /J Build\Tests\TestDeferred\Resources Resources
mklink /J Build\Tests\TestGuis\Resources Resources