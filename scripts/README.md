# Generate .h
Based off of the XML to .H generator found in https://github.com/KhronosGroup/Vulkan-Docs/tree/master/xml
`python genvk.py -registry ./vwsi.xml -o ../include/VWSI vulkan_wsi.h`

# Generate .hpp
Uses the same generator as https://github.com/KhronosGroup/Vulkan-Hpp

# Meta loader
Dynamically load entrypoints https://github.com/zeux/volk
