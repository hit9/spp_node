{
  'targets': [
    {
      'target_name': 'spp',
      'sources': [
        'src/spp.cc',
        'src/parser.cc'
      ],
      'include_dirs': ["<!(node -e \"require('nan')\")"],
      'dependencies': [
        'src/c/spp.gyp:spp'
      ],
      'defines': [
        '_GNU_SOURCE'
      ],
      'cflags': [
        '-Wall',
        '-O3'
      ]
    }
  ]
}
