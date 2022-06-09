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
      "conditions": [
        [
          "OS==\"mac\"",
          {
            "include_dirs": [
              "src",
              "<!(brew --prefix)/include"
            ],
            "libraries": [
              "<!(brew --prefix)/lib/libltc.dylib"
            ]
          }
        ],
        [
          "OS==\"linux\"",
          {
            "link_settings": {
              "libraries": [
                "-lltc"
              ]
            }
          }
        ]
      ]
    }
  ]
}
