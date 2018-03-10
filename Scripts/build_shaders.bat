cd ../

for /R "Resources" %%G in (*.frag) do ( 
	Libraries\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "Resources" %%G in (*.vert) do ( 
	Libraries\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "Resources" %%G in (*.geom) do ( 
	Libraries\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "Resources" %%G in (*.comp) do ( 
	Libraries\glslangValidator.exe -V %%G -o %%G.spv
)
