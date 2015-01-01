{
  'targets': [
    {
      'target_name': 'spp',
      'type': 'static_library',
      'direct_dependent_settings': {
        'include_dirs': [ '.' ],
      },
      'sources': ['./spp.c', './buf.c'],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_C_LANGUAGE_STANDARD': 'c99'
          }
        }],
        ['OS=="solaris"', {
          'cflags+': [ '-std=c99' ]
        }]
      ]
    }
  ]
}
