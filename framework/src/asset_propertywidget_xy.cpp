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

// Include
#include <QVBoxLayout>
#include <QLabel>
#include <QRegExpValidator>
#include "asset_propertywidget_xy.h"

namespace Magus
{
    //****************************************************************************/
    QtXYProperty::QtXYProperty(const QString& title, qreal x, qreal y, QWidget* parent) :
        QtProperty(parent),
        mPrecision(3)
    {
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QHBoxLayout* horizontalLayout = new QHBoxLayout;
        QHBoxLayout* xyLayout = new QHBoxLayout;
        QHBoxLayout* xLayout = new QHBoxLayout;
        QHBoxLayout* yLayout = new QHBoxLayout;
        mainLayout->setContentsMargins(5, 0, 5, 0);
        QLabel* label = new QLabel(title);
        mXlabel = new QLabel(QString("X"));
        mYlabel = new QLabel(QString("Y"));
        mXEdit = new QLineEdit;
        mYEdit = new QLineEdit;
        QRegExp regularExpression("[+-]?([0-9]+\\.([0-9]+)?|\\.[0-9]+)([eE][+-]?[0-9]+)?"); // floating point
        QRegExpValidator* validator = new QRegExpValidator(regularExpression);
        mXEdit->setValidator(validator);
        mYEdit->setValidator(validator);
        setXY(x, y);

        // Layout
        xLayout->addWidget(mXlabel, 1);
        xLayout->addWidget(mXEdit, 100);
        yLayout->addWidget(mYlabel, 1);
        yLayout->addWidget(mYEdit, 100);
        xyLayout->addLayout(xLayout);
        xyLayout->addLayout(yLayout);
        horizontalLayout->addWidget(label, 1);
        horizontalLayout->addLayout(xyLayout, 2);
        mainLayout->addLayout(horizontalLayout);
        setLayout(mainLayout);
        mType = XY;
        connect(mXEdit, SIGNAL(textEdited(QString)), this, SLOT(propertyValueChanged(void)));
        connect(mYEdit, SIGNAL(textEdited(QString)), this, SLOT(propertyValueChanged(void)));
    }

    //****************************************************************************/
    void QtXYProperty::setLabelX (const QString& label)
    {
        mXlabel->setText(label);
    }

    //****************************************************************************/
    void QtXYProperty::setLabelY (const QString& label)
    {
        mYlabel->setText(label);
    }

    //****************************************************************************/
    void QtXYProperty::setXY (qreal x, qreal y)
    {
        setX(x);
        setY(y);
    }

    //****************************************************************************/
    void QtXYProperty::setX (qreal x)
    {
        QString s = QString::number(x, 'f', mPrecision);
        mXEdit->setText(s);
    }

    //****************************************************************************/
    void QtXYProperty::setY (qreal y)
    {
        QString s = QString::number(y, 'f', mPrecision);
        mYEdit->setText(s);
    }

    //****************************************************************************/
    const qreal QtXYProperty::getX (void) const
    {
        double d = mXEdit->text().toDouble();
        return QVariant(d).toReal();
    }

    //****************************************************************************/
    const qreal QtXYProperty::getY (void) const
    {
        double d = mYEdit->text().toDouble();
        return QVariant(d).toReal();
    }

    //****************************************************************************/
    void QtXYProperty::propertyValueChanged(void)
    {
        emit valueChanged(this);
    }
}
