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
          'OS=="win"', {
            "configurations": {
              "Release": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "RuntimeTypeInfo": "true",
                    "ExceptionHandling": 1
                  }
                }
              }
            },
            "libraries": [
               "-l../libltc/bin/libltc.lib"
            ],
            "copies": [
              {
                "destination": "build/Release",
                "files": [
                  "libltc/bin/libltc.dll"
                ]
              }
            ]
          },
        ],
        [
          "OS==\"linux\"", {
            "conditions": [
              ['target_arch=="arm64"', {
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
                      "<@(module_root_dir)/libltc/arm64-bin/libltc.so.11"
                    ]
                  }
                ]
              }],
              ['target_arch=="x64"', {
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
              }],
              ['target_arch=="arm"', { # arm
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
                      "<@(module_root_dir)/libltc/arm-bin/libltc.so.11"
                    ]
                  }
                ]
              }]
            ]
          }
        ]
      ]
    }
  ]
}
