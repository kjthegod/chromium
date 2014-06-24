// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict';

/**
 * Crop mode.
 *
 * @extends {ImageEditor.Mode}
 * @constructor
 */
ImageEditor.Mode.Crop = function() {
  ImageEditor.Mode.call(this, 'crop', 'GALLERY_CROP');
};

ImageEditor.Mode.Crop.prototype = {__proto__: ImageEditor.Mode.prototype};

/**
 * Sets the mode up.
 * @override
 */
ImageEditor.Mode.Crop.prototype.setUp = function() {
  ImageEditor.Mode.prototype.setUp.apply(this, arguments);

  var container = this.getImageView().container_;
  var doc = container.ownerDocument;

  this.domOverlay_ = doc.createElement('div');
  this.domOverlay_.className = 'crop-overlay';
  container.appendChild(this.domOverlay_);

  this.shadowTop_ = doc.createElement('div');
  this.shadowTop_.className = 'shadow';
  this.domOverlay_.appendChild(this.shadowTop_);

  this.middleBox_ = doc.createElement('div');
  this.middleBox_.className = 'middle-box';
  this.domOverlay_.appendChild(this.middleBox_);

  this.shadowLeft_ = doc.createElement('div');
  this.shadowLeft_.className = 'shadow';
  this.middleBox_.appendChild(this.shadowLeft_);

  this.cropFrame_ = doc.createElement('div');
  this.cropFrame_.className = 'crop-frame';
  this.middleBox_.appendChild(this.cropFrame_);

  this.shadowRight_ = doc.createElement('div');
  this.shadowRight_.className = 'shadow';
  this.middleBox_.appendChild(this.shadowRight_);

  this.shadowBottom_ = doc.createElement('div');
  this.shadowBottom_.className = 'shadow';
  this.domOverlay_.appendChild(this.shadowBottom_);

  var cropFrame = this.cropFrame_;
  function addCropFrame(className) {
    var div = doc.createElement('div');
    div.className = className;
    cropFrame.appendChild(div);
  }

  addCropFrame('left top corner');
  addCropFrame('top horizontal');
  addCropFrame('right top corner');
  addCropFrame('left vertical');
  addCropFrame('right vertical');
  addCropFrame('left bottom corner');
  addCropFrame('bottom horizontal');
  addCropFrame('right bottom corner');

  this.onResizedBound_ = this.onResized_.bind(this);
  window.addEventListener('resize', this.onResizedBound_);

  this.createDefaultCrop();
};

/**
 * Handles resizing of the window and updates the crop rectangle.
 * @private
 */
ImageEditor.Mode.Crop.prototype.onResized_ = function() {
  this.positionDOM();
};

/**
 * Resets the mode.
 */
ImageEditor.Mode.Crop.prototype.reset = function() {
  ImageEditor.Mode.prototype.reset.call(this);
  this.createDefaultCrop();
};

/**
 * Updates the position of DOM elements.
 */
ImageEditor.Mode.Crop.prototype.positionDOM = function() {
  var screenClipped = this.viewport_.getScreenClipped();

  var screenCrop = this.viewport_.imageToScreenRect(this.cropRect_.getRect());
  var delta = ImageEditor.Mode.Crop.MOUSE_GRAB_RADIUS;
  this.editor_.hideOverlappingTools(
      screenCrop.inflate(delta, delta),
      screenCrop.inflate(-delta, -delta));

  this.domOverlay_.style.left = screenClipped.left + 'px';
  this.domOverlay_.style.top = screenClipped.top + 'px';
  this.domOverlay_.style.width = screenClipped.width + 'px';
  this.domOverlay_.style.height = screenClipped.height + 'px';

  this.shadowLeft_.style.width = screenCrop.left - screenClipped.left + 'px';

  this.shadowTop_.style.height = screenCrop.top - screenClipped.top + 'px';

  this.shadowRight_.style.width = screenClipped.left + screenClipped.width -
      (screenCrop.left + screenCrop.width) + 'px';

  this.shadowBottom_.style.height = screenClipped.top + screenClipped.height -
      (screenCrop.top + screenCrop.height) + 'px';
};

/**
 * Removes the overlay elements from the document.
 */
ImageEditor.Mode.Crop.prototype.cleanUpUI = function() {
  ImageEditor.Mode.prototype.cleanUpUI.apply(this, arguments);
  this.domOverlay_.parentNode.removeChild(this.domOverlay_);
  this.domOverlay_ = null;
  this.editor_.hideOverlappingTools();
  window.removeEventListener(this.onResizedBound_);
  this.onResizedBound_ = null;
};

/**
 * @const
 * @type {number}
 */
ImageEditor.Mode.Crop.MOUSE_GRAB_RADIUS = 6;

/**
 * @const
 * @type {number}
 */
ImageEditor.Mode.Crop.TOUCH_GRAB_RADIUS = 20;

/**
 * Gets command to do the crop depending on the current state.
 *
 * @return {Command.Crop} Crop command.
 */
ImageEditor.Mode.Crop.prototype.getCommand = function() {
  var cropImageRect = this.cropRect_.getRect();
  return new Command.Crop(cropImageRect);
};

/**
 * Creates default (initial) crop.
 */
ImageEditor.Mode.Crop.prototype.createDefaultCrop = function() {
  var rect = new Rect(this.getViewport().getImageClipped());
  rect = rect.inflate(
      -Math.round(rect.width / 6), -Math.round(rect.height / 6));
  this.cropRect_ = new DraggableRect(rect, this.getViewport());
  this.positionDOM();
};

/**
 * Obtains the cursor style depending on the mouse state.
 *
 * @param {number} x X coordinate for cursor.
 * @param {number} y Y coordinate for cursor.
 * @param {boolean} mouseDown If mouse button is down.
 * @return {string} A value for style.cursor CSS property.
 */
ImageEditor.Mode.Crop.prototype.getCursorStyle = function(x, y, mouseDown) {
  return this.cropRect_.getCursorStyle(x, y, mouseDown);
};

/**
 * Obtains handler function depending on the mouse state.
 *
 * @param {number} x Event X coordinate.
 * @param {number} y Event Y coordinate.
 * @param {boolean} touch True if it's a touch event, false if mouse.
 * @return {function(number,number)} A function to be called on mouse drag.
 */
ImageEditor.Mode.Crop.prototype.getDragHandler = function(x, y, touch) {
  var cropDragHandler = this.cropRect_.getDragHandler(x, y, touch);
  if (!cropDragHandler)
    return null;

  return function(x, y) {
    cropDragHandler(x, y);
    this.markUpdated();
    this.positionDOM();
  }.bind(this);
};

/**
 * Obtains the double tap action depending on the coordinate.
 *
 * @param {number} x X coordinate of the event.
 * @param {number} y Y coordinate of the event.
 * @return {ImageBuffer.DoubleTapAction} Action to perform as result.
 */
ImageEditor.Mode.Crop.prototype.getDoubleTapAction = function(x, y) {
  return this.cropRect_.getDoubleTapAction(x, y);
};

/**
 * A draggable rectangle over the image.
 *
 * @param {Rect} rect Initial size of the image.
 * @param {Viewport} viewport Viewport.
 * @constructor
 */
function DraggableRect(rect, viewport) {
  /**
   * The bounds are not held in a regular rectangle (with width/height).
   * left/top/right/bottom held instead for convenience.
   *
   * @type {{left: number, right: number, top: number, bottom: number}}
   * @private
   */
  this.bounds_ = {};
  this.bounds_[DraggableRect.LEFT] = rect.left;
  this.bounds_[DraggableRect.RIGHT] = rect.left + rect.width;
  this.bounds_[DraggableRect.TOP] = rect.top;
  this.bounds_[DraggableRect.BOTTOM] = rect.top + rect.height;

  /**
   * Viewport.
   *
   * @param {Viewport}
   * @private
   */
  this.viewport_ = viewport;

  /**
   * Drag mode.
   *
   * @type {Object}
   * @private
   */
  this.dragMode_ = null;

  Object.seal(this);
}

// Static members to simplify reflective access to the bounds.
/**
 * @const
 * @type {string}
 */
DraggableRect.LEFT = 'left';

/**
 * @const
 * @type {string}
 */
DraggableRect.RIGHT = 'right';

/**
 * @const
 * @type {string}
 */
DraggableRect.TOP = 'top';

/**
 * @const
 * @type {string}
 */
DraggableRect.BOTTOM = 'bottom';

/**
 * @const
 * @type {string}
 */
DraggableRect.NONE = 'none';

/**
 * Obtains the left position.
 * @return {number} Position.
 */
DraggableRect.prototype.getLeft = function() {
  return this.bounds_[DraggableRect.LEFT];
};

/**
 * Obtains the right position.
 * @return {number} Position.
 */
DraggableRect.prototype.getRight = function() {
  return this.bounds_[DraggableRect.RIGHT];
};

/**
 * Obtains the top position.
 * @return {number} Position.
 */
DraggableRect.prototype.getTop = function() {
  return this.bounds_[DraggableRect.TOP];
};

/**
 * Obtains the bottom position.
 * @return {number} Position.
 */
DraggableRect.prototype.getBottom = function() {
  return this.bounds_[DraggableRect.BOTTOM];
};

/**
 * Obtains the geometory of the rectangle.
 * @return {Rect} Geometory of the rectangle.
 */
DraggableRect.prototype.getRect = function() {
  return new Rect(this.bounds_);
};

/**
 * Obtains the drag mode depending on the coordinate.
 *
 * @param {number} x X coordinate for cursor.
 * @param {number} y Y coordinate for cursor.
 * @param {boolean} touch  Whether the operation is done by touch or not.
 * @return {Object} Drag mode.
 */
DraggableRect.prototype.getDragMode = function(x, y, touch) {
  var result = {
    xSide: DraggableRect.NONE,
    ySide: DraggableRect.NONE
  };

  var bounds = this.bounds_;
  var R = this.viewport_.screenToImageSize(
      touch ? ImageEditor.Mode.Crop.TOUCH_GRAB_RADIUS :
              ImageEditor.Mode.Crop.MOUSE_GRAB_RADIUS);

  var circle = new Circle(x, y, R);

  var xBetween = ImageUtil.between(bounds.left, x, bounds.right);
  var yBetween = ImageUtil.between(bounds.top, y, bounds.bottom);

  if (circle.inside(bounds.left, bounds.top)) {
    result.xSide = DraggableRect.LEFT;
    result.ySide = DraggableRect.TOP;
  } else if (circle.inside(bounds.left, bounds.bottom)) {
    result.xSide = DraggableRect.LEFT;
    result.ySide = DraggableRect.BOTTOM;
  } else if (circle.inside(bounds.right, bounds.top)) {
    result.xSide = DraggableRect.RIGHT;
    result.ySide = DraggableRect.TOP;
  } else if (circle.inside(bounds.right, bounds.bottom)) {
    result.xSide = DraggableRect.RIGHT;
    result.ySide = DraggableRect.BOTTOM;
  } else if (yBetween && Math.abs(x - bounds.left) <= R) {
    result.xSide = DraggableRect.LEFT;
  } else if (yBetween && Math.abs(x - bounds.right) <= R) {
    result.xSide = DraggableRect.RIGHT;
  } else if (xBetween && Math.abs(y - bounds.top) <= R) {
    result.ySide = DraggableRect.TOP;
  } else if (xBetween && Math.abs(y - bounds.bottom) <= R) {
    result.ySide = DraggableRect.BOTTOM;
  } else if (xBetween && yBetween) {
    result.whole = true;
  } else {
    result.newcrop = true;
    result.xSide = DraggableRect.RIGHT;
    result.ySide = DraggableRect.BOTTOM;
  }

  return result;
};

/**
 * Obtains the cursor style depending on the coordinate.
 *
 * @param {number} x X coordinate for cursor.
 * @param {number} y Y coordinate for cursor.
 * @param {boolean} mouseDown  If mouse button is down.
 * @return {string} Cursor style.
 */
DraggableRect.prototype.getCursorStyle = function(x, y, mouseDown) {
  var mode;
  if (mouseDown) {
    mode = this.dragMode_;
  } else {
    mode = this.getDragMode(
        this.viewport_.screenToImageX(x), this.viewport_.screenToImageY(y));
  }
  if (mode.whole)
    return 'move';
  if (mode.newcrop)
    return 'crop';

  var xSymbol = '';
  switch (mode.xSide) {
    case 'left': xSymbol = 'w'; break;
    case 'right': xSymbol = 'e'; break;
  }
  var ySymbol = '';
  switch (mode.ySide) {
    case 'top': ySymbol = 'n'; break;
    case 'bottom': ySymbol = 's'; break;
  }
  return ySymbol + xSymbol + '-resize';
};

/**
 * Obtains the drag handler depeding on the coordinate.
 *
 * @param {number} startScreenX X coordinate for cursor in the screen.
 * @param {number} startScreenY Y coordinate for cursor in the screen.
 * @param {boolean} touch Whether the operaiton is done by touch or not.
 * @return {function(number,number)} Drag handler.
 */
DraggableRect.prototype.getDragHandler = function(
    startScreenX, startScreenY, touch) {
  // Check if the start coordinate in the clip rect.
  var startX = this.viewport_.screenToImageX(startScreenX);
  var startY = this.viewport_.screenToImageY(startScreenY);
  var clipRect = this.viewport_.getImageClipped();
  if (!clipRect.inside(startX, startY))
    return null;

  // Obtain the drag mode.
  this.dragMode_ = this.getDragMode(startX, startY, touch);

  if (this.dragMode_.whole) {
    // Calc constant values during the operation.
    var mouseBiasX = this.bounds_.left - startX;
    var mouseBiasY = this.bounds_.top - startY;
    var initialWidth = this.bounds_.right - this.bounds_.left;
    var initialHeight = this.bounds_.bottom - this.bounds_.top;
    var maxX = clipRect.left + clipRect.width - initialWidth;
    var maxY = clipRect.top + clipRect.height - initialHeight;

    // Returns a handler.
    return function(newScreenX, newScreenY) {
      var newX = this.viewport_.screenToImageX(newScreenX);
      var newY = this.viewport_.screenToImageY(newScreenY);
      var clamppedX = ImageUtil.clamp(clipRect.left, newX + mouseBiasX, maxX);
      var clamppedY = ImageUtil.clamp(clipRect.top, newY + mouseBiasY, maxY);
      this.bounds_.left = clamppedX;
      this.bounds_.right = clamppedX + initialWidth;
      this.bounds_.top = clamppedY;
      this.bounds_.bottom = clamppedY + initialHeight;
    }.bind(this);
  } else {
    // Calc constant values during the operation.
    var mouseBiasX = this.bounds_[this.dragMode_.xSide] - startX;
    var mouseBiasY = this.bounds_[this.dragMode_.ySide] - startY;
    var maxX = clipRect.left + clipRect.width;
    var maxY = clipRect.top + clipRect.height;

    // Returns a handler.
    return function(newScreenX, newScreenY) {
      var newX = this.viewport_.screenToImageX(newScreenX);
      var newY = this.viewport_.screenToImageY(newScreenY);

      // Check new crop.
      if (this.dragMode_.newcrop) {
        this.dragMode_.newcrop = false;
        this.bounds_.left = this.bounds_.right = newX;
        this.bounds_.top = this.bounds_.bottom = newY;
        mouseBiasX = 0;
        mouseBiasY = 0;
      }

      // Update X coordinate.
      if (this.dragMode_.xSide !== DraggableRect.NONE) {
        this.bounds_[this.dragMode_.xSide] =
            ImageUtil.clamp(clipRect.left, newX + mouseBiasX, maxX);
        if (this.bounds_.left > this.bounds_.right) {
          var left = this.bounds_.left;
          var right = this.bounds_.right;
          this.bounds_.left = right;
          this.bounds_.right = left;
          this.dragMode_.xSide =
              this.dragMode_.xSide == 'left' ? 'right' : 'left';
        }
      }

      // Update Y coordinate.
      if (this.dragMode_.ySide !== DraggableRect.NONE) {
        this.bounds_[this.dragMode_.ySide] =
            ImageUtil.clamp(clipRect.top, newY + mouseBiasY, maxY);
        if (this.bounds_.top > this.bounds_.bottom) {
          var top = this.bounds_.top;
          var bottom = this.bounds_.bottom;
          this.bounds_.top = bottom;
          this.bounds_.bottom = top;
          this.dragMode_.ySide =
              this.dragMode_.ySide === 'top' ? 'bottom' : 'top';
        }
      }
    }.bind(this);
  }
};

/**
 * Obtains double tap action depending on the coordinate.
 *
 * @param {number} x X coordinate for cursor.
 * @param {number} y Y coordinate for cursor.
 * @param {boolean} touch Whether the operation is done by touch or not.
 * @return {ImageBuffer.DoubleTapAction} Double tap action.
 */
DraggableRect.prototype.getDoubleTapAction = function(x, y, touch) {
  x = this.viewport_.screenToImageX(x);
  y = this.viewport_.screenToImageY(y);

  var clipRect = this.viewport_.getImageClipped();
  if (clipRect.inside(x, y))
    return ImageBuffer.DoubleTapAction.COMMIT;
  else
    return ImageBuffer.DoubleTapAction.NOTHING;
};
