# -*- encoding: utf-8 -*-
"""
Copyright (c) 2019 - present AppSeed.us
"""

from flask_login import UserMixin
from sqlalchemy import Binary, Column, Integer, String

from app import db


class GameMap(db.Model):
    __tablename__ = 'GameMap'
    map_id = Column(Integer, primary_key=True)
    map_mode = Column(String(7), unique=False, nullable=False)
    map_string= Column(String(200), unique=True, nullable=False)

    def __init__(self, map_id, map_mode, map_string):
        self.map_id = map_id
        self.map_mode = map_mode
        self.map_string = map_string
    
    def getMapId(self):
        return self.map_id
    
    def getMapMode(self):
        return self.map_mode
    
    def getMapString(self):
        return self.map_string

    def setMapId(self, map_id):
        self.map_id = map_id
    
    def setMapMode(self, map_mode):
        self.map_mode = map_mode
    
    def setMapString(self, map_string):
        self.map_string = map_string
    
    def __repr__(self):
        return f"GameMap('{self.map_mode}','{self.map_string}')"
