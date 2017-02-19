/****************************************************************************
**
** Copyright (C) 2016
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef MAGUS_PROPERTY_WIDGET_SLIDER_DECIMAL_H
#define MAGUS_PROPERTY_WIDGET_SLIDER_DECIMAL_H

#include <QWidget>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include "asset_propertywidget.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace Magus
{
    /****************************************************************************
    Property class for decimal values, using a slider
    ***************************************************************************/
    class QtSliderDecimalProperty : public QtProperty
    {
        Q_OBJECT

        public:
            QtSliderDecimalProperty(const QString& title, QWidget* parent = 0);
            virtual ~QtSliderDecimalProperty(void) {}
            void setSliderRange (float min, float max, float step);
            void setValue (float value);
            float getValue (void) const;
            void setTitle(const QString title);
            void setPrecision(int precision);
            int getPrecision(void);

        private slots:
            void editValueChanged(void);
            void sliderValueChanged(void);

        private:
            QSlider* mSlider;
            QLineEdit* mEdit;
            QLabel* mTitle;
            float mMin;
            float mMax;
            int mMinRange;
            int mMaxRange;
            float mStep;
            int mPrecision;
    };
}

#endif
