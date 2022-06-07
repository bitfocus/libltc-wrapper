{
  'targets': [
    {
      'target_name': 'ltc-native',
      'sources': [ 'src/ltcnative.c' ],
      'cflags!': [ '-fno-exceptions' ],
      'linkflags!': [ '-lltc' ],
      'conditions': [
        ['OS=="mac"', {
          'include_dirs': [ 'src', '<!(brew --prefix)/include' ],
          'libraries': [ '<!(brew --prefix)/lib/libltc.dylib' ]
        }]
      ]
    }
  ]
}