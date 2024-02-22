{
    "targets": [{
        "target_name": "testaddon",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "cppsrc/main.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    },
    {



      "target_name": "testaddon_wasm",
      "sources": [ "cppsrc/main.cpp" ],
        "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
      "cflags!": [ "-fno-exceptions", "-s WASM=1" ],
      "cflags_cc!": [ "-fno-exceptions", "-s WASM=1" ],
    }


    ]
}
