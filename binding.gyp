{
  "targets": [
    {
      "target_name": "ltc-native",
      "sources": [
        "src/ltcnative.c"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "include_dirs": [
        "libltc/include"
      ],
      "conditions": [
        [
          "OS==\"mac\"",
          {
            "link_settings": {
              "libraries": [
                "-Wl,-rpath,@loader_path"
              ],
              "ldflags": [
                "-L<@(module_root_dir)/build/Release",
                "-Wl,-rpath,<@(module_root_dir)/build/Release"
              ]

            },
            "libraries": [
              "libltc.11.dylib"
            ],
            "copies": [
              {
                "destination": "build/Release/",
                "files": [
                  "<!@(ls -1 libltc/bin/libltc.11.dylib)"
                ]
              }
            ],
            'postbuilds': [
              {
                'postbuild_name': '@rpath for libltc',
                'action': [
                  'install_name_tool',
                  '-change',
                  '/usr/local/lib/libltc.11.dylib',
                  '@rpath/libltc.11.dylib',
                  '<@(module_root_dir)/build/Release/ltc-native.node'
                ],
              },
            ]
          }
        ],
        [
          "OS==\"linux\"",
          {
            "link_settings": {
              "libraries": [
                "<@(module_root_dir)/build/Release/libltc.so.11"
              ],
              "ldflags": [
                "-L<@(module_root_dir)/build/Release",
                "-Wl,-rpath,<@(module_root_dir)/build/Release"
              ]
            },
            "copies": [
              {
                "destination": "build/Release/",
                "files": [
                  "<@(module_root_dir)/libltc/bin/libltc.so.11"
                ]
              }
            ]
          }
        ]
      ]
    }
  ]
}
