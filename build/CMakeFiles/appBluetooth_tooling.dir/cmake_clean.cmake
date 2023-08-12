file(REMOVE_RECURSE
  "Bluetooth/ui/Main.qml"
  "Bluetooth/ui/screens/HomeScreen.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appBluetooth_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
