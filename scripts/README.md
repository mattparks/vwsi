# Generate .h
Based off of the XML to .H generator found in [Vulkan Docs](https://github.com/KhronosGroup/Vulkan-Docs/tree/master/xml).

First extract pyscripts.zip into the scripts/py/ folder, then run: `python genvk.py -registry ./../vwsi.xml -o ../../include/VWSI vulkan_wsi.h`

# Generate .hpp
To generate a .hpp version of VWSI use [Vulkan Hpp](https://github.com/KhronosGroup/Vulkan-Hpp).

# Meta loader
To dynamically load VWSI entrypoints use [volk](https://github.com/zeux/volk).
