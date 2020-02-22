#!/usr/bin/env python
# -*- coding: utf-8 -*-

from core.mlloader import MLLoader
from core.mlfunction import MLFunction

from exchange.mlptrainer import MLPTrainer
from exchange.mlpnetwork import MLPNetwork
from exchange.mlpmetada import MLPMetaData

import ctypes

def enum(*args):
    values = dict(zip(args, range(len(args))))
    return type('Enum', (), values)

class MLPLoader:
    def __init__(self):
        loader = MLLoader('MLP')

        self.mlp_plugin_init                       = MLFunction(loader, 'mlp_plugin_init',                         None,                       [])
        self.mlp_plugin_metadata                   = MLFunction(loader, 'mlp_plugin_metadata',                     ctypes.POINTER(MLPMetaData),[])
        self.mlp_trainer_new                       = MLFunction(loader, 'mlp_trainer_new',                         ctypes.POINTER(MLPTrainer), [ctypes.c_char_p, ctypes.c_char_p])
        self.mlp_trainer_delete                    = MLFunction(loader, 'mlp_trainer_delete',                      None,                       [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_configure                 = MLFunction(loader, 'mlp_trainer_configure',                   None,                       [ctypes.POINTER(MLPTrainer), ctypes.c_char_p])
        self.mlp_trainer_is_running                = MLFunction(loader, 'mlp_trainer_is_running',                  ctypes.c_ubyte,             [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_get_progress              = MLFunction(loader, 'mlp_trainer_get_progress',                ctypes.c_double,            [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_run                       = MLFunction(loader, 'mlp_trainer_run',                         None,                       [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_error                     = MLFunction(loader, 'mlp_trainer_error',                       ctypes.c_double,            [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_save_progression          = MLFunction(loader, 'mlp_trainer_save_progression',            None,                       [ctypes.POINTER(MLPTrainer), ctypes.c_char_p])
        self.mlp_trainer_restore_progression       = MLFunction(loader, 'mlp_trainer_restore_progression',         None,                       [ctypes.POINTER(MLPTrainer), ctypes.c_char_p, ctypes.c_double, ctypes.c_double])
        self.mlp_trainer_get_network               = MLFunction(loader, 'mlp_trainer_get_network',                 ctypes.POINTER(MLPNetwork), [ctypes.POINTER(MLPTrainer)])
        self.mlp_trainer_get_layer_output_signal   = MLFunction(loader, 'mlp_trainer_get_layer_output_signal',     None,                       [ctypes.POINTER(MLPTrainer), ctypes.c_uint], True)
        self.mlp_trainer_get_input_signal          = MLFunction(loader, 'mlp_trainer_get_input_signal',            None,                       [ctypes.POINTER(MLPTrainer)], True)

        self.mlp_network_new                       = MLFunction(loader, 'mlp_network_new',                         ctypes.POINTER(MLPNetwork), [ctypes.c_char_p])
        self.mlp_network_delete                    = MLFunction(loader, 'mlp_network_delete',                      None,                       [ctypes.POINTER(MLPNetwork)])
        self.mlp_network_serialize                 = MLFunction(loader, 'mlp_network_serialize',                   None,                       [ctypes.POINTER(MLPNetwork), ctypes.c_char_p])
        self.mlp_network_deserialize               = MLFunction(loader, 'mlp_network_deserialize',                 None,                       [ctypes.POINTER(MLPNetwork), ctypes.c_char_p])
        self.mlp_network_predict                   = MLFunction(loader, 'mlp_network_predict',                     None,                       [ctypes.POINTER(MLPNetwork), ctypes.c_uint, ctypes.c_void_p])
        self.mlp_network_get_output                = MLFunction(loader, 'mlp_network_get_output',                  ctypes.c_void_p,            [ctypes.POINTER(MLPNetwork)])
        self.mlp_network_get_output_length         = MLFunction(loader, 'mlp_network_get_output_length',           ctypes.c_uint,              [ctypes.POINTER(MLPNetwork)])
        self.mlp_network_get_layer_number_of_neuron= MLFunction(loader, 'mlp_network_get_layer_number_of_neuron',  ctypes.c_uint,              [ctypes.POINTER(MLPNetwork), ctypes.c_uint])
        self.mlp_network_get_number_of_layer       = MLFunction(loader, 'mlp_network_get_number_of_layer',         ctypes.c_uint,              [ctypes.POINTER(MLPNetwork)])
        self.mlp_network_get_number_of_input       = MLFunction(loader, 'mlp_network_get_number_of_input',         ctypes.c_uint,              [ctypes.POINTER(MLPNetwork)])
