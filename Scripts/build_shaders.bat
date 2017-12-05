for /R "C:\Users\mattp\Documents\Flounder\Sources" %%G in (*.frag) do ( 
	C:\VulkanSDK\1.0.65.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Sources" %%G in (*.vert) do ( 
	C:\VulkanSDK\1.0.65.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Sources" %%G in (*.geom) do ( 
	C:\VulkanSDK\1.0.65.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Sources" %%G in (*.comp) do ( 
	C:\VulkanSDK\1.0.65.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

pause
