# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('deficit-round-robin', ['core'])
    module.source = [
        'model/deficit-round-robin.cc',
        'helper/deficit-round-robin-helper.cc',
        'model/custom-packet-sink.cc',
        ]

    module_test = bld.create_ns3_module_test_library('deficit-round-robin')
    module_test.source = [
        'test/deficit-round-robin-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'deficit-round-robin'
    headers.source = [
        'model/deficit-round-robin.h',
        'helper/deficit-round-robin-helper.h',
        'model/custom-packet-sink.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

