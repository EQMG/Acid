for /R "C:\Users\mattp\Documents\Flounder\Resources" %%G in (*.frag) do ( 
	C:\VulkanSDK\1.0.68.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Resources" %%G in (*.vert) do ( 
	C:\VulkanSDK\1.0.68.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Resources" %%G in (*.geom) do ( 
	C:\VulkanSDK\1.0.68.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)

for /R "C:\Users\mattp\Documents\Flounder\Resources" %%G in (*.comp) do ( 
	C:\VulkanSDK\1.0.68.0\Bin32\glslangValidator.exe -V %%G -o %%G.spv
)
