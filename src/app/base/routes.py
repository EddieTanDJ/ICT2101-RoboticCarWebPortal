from flask import jsonify, render_template, redirect, request, url_for

from app import db
from app.base import blueprint
from app.base.models import GameMap

from app.base.util import verify_pass

@blueprint.route('/')
def route_default():
    return render_template('Play.html')

@blueprint.errorhandler(403)
def access_forbidden(error):
    return render_template('page-403.html'), 403

@blueprint.errorhandler(404)
def not_found_error(error):
    return render_template('page-404.html'), 404

@blueprint.errorhandler(500)
def internal_error(error):
    return render_template('page-500.html'), 500



