/*ckwg +29
 * Copyright 2015 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "NeckerReversalTool.h"

#include <maptk/transform.h>

#include <QtGui/QMessageBox>

//-----------------------------------------------------------------------------
NeckerReversalTool::NeckerReversalTool(QObject* parent)
  : AbstractTool(parent)
{
  this->setText("Reverse (&Necker)");
  this->setToolTip(
    "Attempt to correct a degenerate refinement (which can occur due to the "
    "Necker cube phenomena) by rotating the cameras 180&deg; about the axis "
    "formed by the mean landmark position and mean camera center and 180&deg; "
    "about their principal axes, and mirroring landmarks about the plane at "
    "the mean landmark position end of the same axis");
}

//-----------------------------------------------------------------------------
NeckerReversalTool::~NeckerReversalTool()
{
}

//-----------------------------------------------------------------------------
AbstractTool::Outputs NeckerReversalTool::outputs() const
{
  return Cameras | Landmarks;
}

//-----------------------------------------------------------------------------
bool NeckerReversalTool::execute(QWidget* window)
{
  if (!this->hasLandmarks() || !this->hasCameras())
  {
    QMessageBox::information(
      window, "Insufficient data",
      "This operation requires landmarks and cameras.");
    return false;
  }

  return AbstractTool::execute(window);
}

//-----------------------------------------------------------------------------
void NeckerReversalTool::run()
{
  auto cp = this->cameras();
  auto lp = this->landmarks();

  kwiver::maptk::necker_reverse(cp, lp);

  this->updateCameras(cp);
  this->updateLandmarks(lp);
}
