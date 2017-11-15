/* -*- coding: utf-8-unix -*-
 *
 * Copyright (C) 2014 Osmo Salomaa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.



 Copied from https://github.com/otsaloma/poor-maps/qml/js/util.js, version 0.33.

 Modifications (C) 2017 Jukka Sirkka


*/

.import QtPositioning 5.3 as P

function home() {
    var homeLat = 37.13
    var homeLng = -8.54
    return P.QtPositioning.coordinate(homeLat, homeLng)
}

var constants = {
    canvasTileSize: 512,
    canvasScaleFactor: 0.5,
    halfZoom: 0.5849625
}

function scaleDisplay(x) {
    var n = 2

    // Round x to n significant digits.
    var mult = Math.pow(10, n - Math.floor(Math.log(x) / Math.LN10) - 1)
    var disp = Math.round(x * mult) / mult

    var str = "" + disp + " m"
    if (x >= 10000) {
        str = "" + disp / 1000 + " km"
    }
    return {'string': str,'value': disp}
}

/*
 * Calculations below between longitude/latitude coordinates,
 * Mercator-projected coordinates and pixel positions on screen
 * assume a spherical Mercator map canvas.
 *
 * http://en.wikipedia.org/wiki/Mercator_projection
 * http://wiki.openstreetmap.org/wiki/Mercator
 */


function eucd(x1, y1, x2, y2) {
    // Calculate euclidean distance.
    var xd = x2 - x1;
    var yd = y2 - y1;
    return Math.sqrt(xd*xd + yd*yd);
}


function deg2rad(deg) {
    // Convert degrees to radians.
    return deg / 180 * Math.PI;
}


function rad2deg(rad) {
    // Convert radians to degrees.
    return rad / Math.PI * 180;
}


function xcoord2xpos(x, xmin, xmax, width) {
    // Convert X-coordinate to pixel X-position on screen.
    return Math.round((x - xmin) * (width / (xmax - xmin)));
}

function xpos2xcoord(x, xmin, xmax, width) {
    // Convert screen pixel X-position to X-coordinate.
    return xmin + (x / (width / (xmax - xmin)));
}

function ycoord2ymercator(y) {
    // Convert Y-coordinate to Mercator projected Y-coordinate.
    return Math.log(Math.tan(Math.PI/4 + deg2rad(y)/2));
}

function ycoord2ypos(y, ymin, ymax, height) {
    // Convert Y-coordinate to pixel Y-position on screen.
    ymin = ycoord2ymercator(ymin);
    ymax = ycoord2ymercator(ymax);
    return Math.round((ymax - ycoord2ymercator(y)) * (height / (ymax - ymin)));
}

function ymercator2ycoord(y) {
    // Convert Mercator projected Y-coordinate to Y-coordinate.
    return rad2deg(2 * Math.atan(Math.exp(y)) - Math.PI/2);
}

function ypos2ycoord(y, ymin, ymax, height) {
    // Convert screen pixel Y-position to Y-coordinate.
    ymin = ymercator2ycoord(ymin);
    ymax = ymercator2ycoord(ymax);
    return (ymin + (ymercator2ycoord(y) / (height / (ymax - ymin))));
}
