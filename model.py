########################################################################
# ======================  TrackML CHALLENGE MODEL  =====================
########################################################################
# Author: Marcel Kunze
# Date: Dec. 2018

from __future__ import print_function
from __future__ import division
from __future__ import absolute_import
from __future__ import unicode_literals

import numpy as np
import pandas as pd
import os
import ctypes

_dllPath = os.path.dirname(__file__)
libPath = os.path.join(_dllPath, 'libmodel.so')
_model = ctypes.cdll.LoadLibrary(libPath)
#_model = ctypes.CDLL(libPath)

_workPath = os.path.dirname(os.path.realpath(__file__))
print('model.py:' + _workPath)

_model.processInitHits.argtypes = (ctypes.c_int, ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int))
_model.processInitCells.argtypes = (ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_double))
_model.processSetup.argtypes = (ctypes.c_int,ctypes.c_char_p,ctypes.c_char_p)

def setup(number,datapath):
    global _model
    global _workPath
    c_string = _workPath.encode('utf-8')
    d_string = datapath.encode('utf-8')
    _model.processSetup(ctypes.c_int(number),d_string,c_string)

def initHits(nhits,x,y,z,v,l,m):
    global _model
    array_type1 = ctypes.c_double * nhits
    array_type2 = ctypes.c_double * nhits
    array_type3 = ctypes.c_double * nhits
    array_type4 = ctypes.c_int * nhits
    array_type5 = ctypes.c_int * nhits
    array_type6 = ctypes.c_int * nhits
    result = _model.processInitHits(ctypes.c_int(nhits),array_type1(*x),array_type2(*y),array_type3(*z),array_type4(*v),array_type5(*l),array_type6(*m))

def initCells(ncells,hit_id,ch0,ch1,value):
    global _model
    array_type1 = ctypes.c_int * ncells
    array_type2 = ctypes.c_int * ncells
    array_type3 = ctypes.c_int * ncells
    array_type4 = ctypes.c_double * ncells
    result = _model.processInitCells(ctypes.c_int(ncells),array_type1(*hit_id),array_type2(*ch0),array_type3(*ch1),array_type4(*value))

def findTracks():
    global _model
    global _workPath
    step = 1
    c_string = _workPath.encode('utf-8')
    result = _model.processFindTracks(ctypes.c_int(step),c_string)

def readTruth():
    global _model
    _model.processReadTruth()

def readStarts():
    global _model
    _model.processReadStarts()

def readHits():
    global _model
    _model.processReadHits()

def readCells():
    global _model
    _model.processReadCells()

def readBlacklist():
    global _model
    _model.processReadBlacklist()

def readWhitelist():
    global _model
    _model.processReadWhitelist()

def sortTracks():
    global _model
    _model.processSortTracks()

def finish():
    global _model
    _model.processFinish()

class Model:

    def __init__(self):
        self.workpath = os.path.dirname(os.path.realpath(__file__))
        self.datapath = os.path.dirname(os.path.realpath(__file__))

    def predict_one_event(self, event_id, event, cells):
        
        # Instantiate the tracker
        setup(event_id,self.datapath)

        # Read hits data
        x = event.x.values
        y = event.y.values
        z = event.z.values
        v = event.volume_id.values
        l = event.layer_id.values
        m = event.module_id.values
        nhits = event.shape[0]
        initHits(nhits,x, y, z, v, l, m)
        
        # Read cells data
        hit_id  = cells.hit_id.values
        ch0 = cells.ch0.values
        ch1 = cells.ch1.values
        value = cells.value.values
        ncells = cells.shape[0]
        initCells(ncells,hit_id,ch0,ch1,value)
        
        # Run the traking code
        findTracks()
        
        # Delete the tracker
        finish()
        
        # Read the submission file
        filename = self.workpath+'/submission'+str(event_id)+'.csv'
        sub = pd.read_csv(filename);
        
        return sub
