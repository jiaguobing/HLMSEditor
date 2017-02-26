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
#include <QMessageBox>
#include <QLabel>
#include "asset_assetwidget.h"

namespace Magus
{
    //****************************************************************************/
    QtAssetWidget::QtAssetWidget(const QString& title, const QString& fileNameIcon, QWidget* parent) : QWidget(parent)
    {
        mFileNameIconCollapsed = QString("");
        mHeaderTitleFileNameIcon = QString("");
        mFileNameIconExpand = QString("");

        // Main layout
        mMainLayout = new QVBoxLayout;
        mMainLayout->setContentsMargins(0, 0, 0, 0);

        // Title icon
        mHeaderTitleIconLabel = new QLabel;
        mHeaderTitleIconSize = QSize(DEFAULT_ICON_DIMENSION, DEFAULT_ICON_DIMENSION);
        mHeaderTitleIconLabel->setMinimumHeight(DEFAULT_ICON_DIMENSION);
        mHeaderTitleIconLabel->setMinimumWidth(DEFAULT_ICON_DIMENSION);

        // Action 1 icon
        mHeaderAction1IconLabel = new QtClickableLabel();
        mHeaderAction1IconLabel->setVisible(false);
        mHeaderAction1FileNameIcon = QString("");
        mHeaderAction1IconSize = QSize(DEFAULT_ICON_DIMENSION, DEFAULT_ICON_DIMENSION);
        mHeaderAction1IconLabel->setMinimumHeight(DEFAULT_ICON_DIMENSION);
        mHeaderAction1IconLabel->setMinimumWidth(DEFAULT_ICON_DIMENSION);

        // Action 2 icon
        mHeaderAction2IconLabel = new QtClickableLabel();
        mHeaderAction2IconLabel->setVisible(false);
        mHeaderAction2FileNameIcon = QString("");
        mHeaderAction2IconSize = QSize(DEFAULT_ICON_DIMENSION, DEFAULT_ICON_DIMENSION);
        mHeaderAction2IconLabel->setMinimumHeight(DEFAULT_ICON_DIMENSION);
        mHeaderAction2IconLabel->setMinimumWidth(DEFAULT_ICON_DIMENSION);

        // Header
        mHeader = new QFrame;
        mHeader->setFrameStyle(QFrame::NoFrame);
        mHeader->setFrameShadow(QFrame::Plain);
        mHeader->setFrameShape(QFrame::NoFrame);
        mHeader->setMaximumHeight(ASSET_HEADER_HEIGHT);
        mHeader->setMinimumHeight(ASSET_HEADER_HEIGHT);
        mHeader->setAutoFillBackground(true);
        mHeader->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #686868, stop:0.8 #686868, stop:0.9 #474747, stop:1 #202020);"
                                       "margin: 0px;"
                                       "border: 1px solid #353535;")); // TODO: Put in file?

        // Header: Title
        mHeaderTitleLabel = new QLabel(title);
        mHeaderTitleLabel->setStyleSheet(QString("background: transparent;"
                                                 "border: 0px solid #353535;"));
        setHeaderTitleIcon(fileNameIcon);
        setHeaderTitleIconSize(mHeaderTitleIconSize);

        // Header: Layout
        mHeaderLayout = new QHBoxLayout;
        mHeaderLeftLayout = new QHBoxLayout;
        mHeaderRightLayout = new QHBoxLayout;
        mHeaderLeftLayout->addWidget(mHeaderTitleIconLabel, 0, Qt::AlignLeft);
        mHeaderLeftLayout->addWidget(mHeaderTitleLabel, 0, Qt::AlignLeft);
        mHeaderLeftLayout->addStretch(1000);
        mHeaderRightLayout->addStretch(1000);
        mHeaderRightLayout->addWidget(mHeaderAction1IconLabel, 0, Qt::AlignRight);
        mHeaderRightLayout->addWidget(mHeaderAction2IconLabel, 0, Qt::AlignRight);
        mHeaderLayout->addLayout(mHeaderLeftLayout);
        mHeaderLayout->addLayout(mHeaderRightLayout);
        mHeader->setLayout(mHeaderLayout);
        mHeader->setLayout(mHeaderLayout);

        // Add header to the main layout
        mMainLayout->addWidget(mHeader, 0, Qt::AlignTop);
        mMainLayout->addWidget(mHeader);
        setLayout(mMainLayout);
    }

    //****************************************************************************/
    QtAssetWidget::~QtAssetWidget(void)
    {
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderHidden(bool hidden)
    {
        mHeader->setVisible(!hidden);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderTitle(const QString& title)
    {
        mHeaderTitleLabel->setText(title);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderTitleIcon(const QString& fileNameIcon)
    {
        mHeaderTitleFileNameIcon = fileNameIcon;
        mHeaderTitleIconLabel->setPixmap(QPixmap(fileNameIcon).scaled(
                                        mHeaderTitleIconSize.width(),
                                        mHeaderTitleIconSize.height(),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation));
        mHeaderTitleIconLabel->setStyleSheet(QString("background: transparent;"
                                                     "border: 0px solid #353535;"));
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderAction1Icon(const QString& fileNameIcon)
    {
        mHeaderAction1FileNameIcon = fileNameIcon;
        mHeaderAction1IconLabel->setPixmap(QPixmap(fileNameIcon).scaled(mHeaderAction1IconSize.width(),
                                                                        mHeaderAction1IconSize.height(),
                                                                        Qt::KeepAspectRatio,
                                                                        Qt::SmoothTransformation));
        mHeaderAction1IconLabel->setVisible(true);
        mHeaderAction1IconLabel->setStyleSheet(QString("background: transparent;"
                                                    "border: 0px solid #353535;"));
        connect(mHeaderAction1IconLabel, SIGNAL(clicked(void)), this, SLOT(headerAction1IconClicked(void)));
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderAction2Icon(const QString& fileNameIcon)
    {
        mHeaderAction2FileNameIcon = fileNameIcon;
        mHeaderAction2IconLabel->setPixmap(QPixmap(fileNameIcon).scaled(mHeaderAction2IconSize.width(),
                                                                        mHeaderAction2IconSize.height(),
                                                                        Qt::KeepAspectRatio,
                                                                        Qt::SmoothTransformation));
        mHeaderAction2IconLabel->setVisible(true);
        mHeaderAction2IconLabel->setStyleSheet(QString("background: transparent;"
                                                    "border: 0px solid #353535;"));
        connect(mHeaderAction2IconLabel, SIGNAL(clicked(void)), this, SLOT(headerAction2IconClicked(void)));
    }

    //****************************************************************************/
    void QtAssetWidget::headerAction1IconClicked(void)
    {
        emit headerAction1Clicked();
    }

    //****************************************************************************/
    void QtAssetWidget::headerAction2IconClicked(void)
    {
        emit headerAction2Clicked();
    }

    //****************************************************************************/
    void QtAssetWidget::containerAction1IconClicked(QtContainerWidget* containerWidget)
    {
        emit containerAction1Clicked(containerWidget->mContainerId);
        emit containerAction1Clicked(containerWidget);
    }

    //****************************************************************************/
    void QtAssetWidget::containerAction2IconClicked(QtContainerWidget* containerWidget)
    {
        emit containerAction2Clicked(containerWidget->mContainerId);
        emit containerAction2Clicked(containerWidget);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderTitleIconSize(const QSize& size)
    {
        mHeaderTitleIconSize = size;
        setHeaderTitleIcon(mHeaderTitleFileNameIcon);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderAction1IconSize(const QSize& size)
    {
        mHeaderAction1IconSize = size;
        setHeaderAction1Icon(mHeaderAction1FileNameIcon);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderAction2IconSize(const QSize& size)
    {
        mHeaderAction2IconSize = size;
        setHeaderAction2Icon(mHeaderAction2FileNameIcon);
    }

    //****************************************************************************/
    void QtAssetWidget::setHeaderTitleBold(bool bold)
    {
        QFont font = mHeaderTitleLabel->font();
        font.setBold(bold);
        mHeaderTitleLabel->setFont(font);
    }

    //****************************************************************************/
    void QtAssetWidget::propertyValueChanged(QtProperty* property)
    {
        emit valueChanged(property->mPropertyId);
        emit valueChanged(property);
    }

    //****************************************************************************/
    void QtAssetWidget::rebuildWidgetContent(void)
    {
        if (!mMainLayout)
            return;

        delete mMainLayout;
        mMainLayout = new QVBoxLayout;
        mMainLayout->setContentsMargins(0, 0, 0, 0);

        // Add header
        mMainLayout->addWidget(mHeader, 0, Qt::AlignTop);

        // Iterate
        QMap<int, QtContainerWidget*>::iterator i;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            mMainLayout->addWidget(i.value(), 1, Qt::AlignTop);
        }

        mMainLayout->addStretch(1000);
        setLayout(mMainLayout);
    }

    //****************************************************************************/
    QtContainerWidget* QtAssetWidget::createContainer (int containerId,
                                                       QString title,
                                                       bool line)
    {
        // First check if the container already exists
        if (mQtContainerWidgetMap.contains(containerId))
            return 0;

        // Create it
        QtContainerWidget* containerWidget = new QtContainerWidget(title, line);
        containerWidget->mContainerId = containerId;
        containerWidget->setFileNameIconCollapsed(mFileNameIconCollapsed);
        containerWidget->setFileNameIconExpanded(mFileNameIconExpand);
        mQtContainerWidgetMap[containerId] = containerWidget;
        rebuildWidgetContent();
        connect(containerWidget, SIGNAL(action1Clicked(QtContainerWidget*)), this, SLOT(containerAction1IconClicked(QtContainerWidget*)));
        connect(containerWidget, SIGNAL(action2Clicked(QtContainerWidget*)), this, SLOT(containerAction2IconClicked(QtContainerWidget*)));
        connect(containerWidget, SIGNAL(valueChanged(QtProperty*)), this, SLOT(propertyValueChanged(QtProperty*)));
        return containerWidget;
    }

    //****************************************************************************/
    QtProperty* QtAssetWidget::createProperty (int containerId,
                                               int propertyId,
                                               QString title,
                                               QtProperty::PropertyType type,
                                               bool immutable)
    {
        QtContainerWidget* containerWidget = 0;
        QtProperty* property = 0;
        containerWidget = mQtContainerWidgetMap.value(containerId);
        if (containerWidget)
        {
            // Do not connect to this property directly, valueChanged signals are emitted via the container
            property = containerWidget->createProperty(propertyId, title, type, immutable);
            property->mPropertyId = propertyId;
        }

        return property;
    }

    //****************************************************************************/
    QtContainerWidget* QtAssetWidget::getContainerWidget (int containerId)
    {
        return mQtContainerWidgetMap.value(containerId);
    }

    //****************************************************************************/
    QtProperty* QtAssetWidget::getPropertyWidget (int containerId, int propertyId)
    {
        QtContainerWidget* container = mQtContainerWidgetMap.value(containerId);
        if (container)
        {
            return container->getPropertyWidget (propertyId);
        }
        return 0;
    }

    //****************************************************************************/
    QtProperty* QtAssetWidget::getPropertyWidget (int propertyId)
    {
        // Iterate through containers and get the first (property should have a unique Id)
        QtContainerWidget* container = 0;
        QtProperty* property = 0;
        QMap<int, QtContainerWidget*>::iterator i;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            container = i.value();
            if (container)
            {
                property = container->getPropertyWidget (propertyId);
                if (property)
                {
                    return property;
                }
            }
        }
        return 0;
    }

    //****************************************************************************/
    void QtAssetWidget::setFileNameIconCollapsed(const QString& fileNameIcon)
    {
        mFileNameIconCollapsed = fileNameIcon;

        // Iterate through containers
        QMap<int, QtContainerWidget*>::iterator i;
        QtContainerWidget* widget = 0;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            widget = i.value();
            widget->setFileNameIconCollapsed(fileNameIcon);
        }
    }

    //****************************************************************************/
    void QtAssetWidget::setFileNameIconExpanded(const QString& fileNameIcon)
    {
        mFileNameIconExpand = fileNameIcon;

        // Iterate through containers
        QMap<int, QtContainerWidget*>::iterator i;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            QtContainerWidget* widget = i.value();
            widget->setFileNameIconExpanded(fileNameIcon);
        }
    }

    //****************************************************************************/
    void QtAssetWidget::expandAllContainers(void)
    {
        // Iterate through containers
        QMap<int, QtContainerWidget*>::iterator i;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            QtContainerWidget* widget = i.value();
            widget->expand();
        }
    }

    //****************************************************************************/
    void QtAssetWidget::collapseAllContainers(void)
    {
        // Iterate through containers
        QMap<int, QtContainerWidget*>::iterator i;
        for (i = mQtContainerWidgetMap.begin(); i != mQtContainerWidgetMap.end(); ++i)
        {
            QtContainerWidget* widget = i.value();
            widget->collapse();
        }
    }

    //****************************************************************************/
    void QtAssetWidget::setContainerTitleBold(int containerId, bool bold)
    {
        QtContainerWidget* widget = mQtContainerWidgetMap[containerId];
        widget->setTitleBold(bold);
    }

    //****************************************************************************/
    void QtAssetWidget::useContextMenu(bool useContextMenu)
    {
        if (useContextMenu)
        {
            if (mContextMenu)
                return;

            mContextMenu = new QMenu(this);
            setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
            connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(itemSelected(QAction*)));
        }
        else
        {
            if (!mContextMenu)
                return;

            disconnect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
            setContextMenuPolicy(Qt::DefaultContextMenu);
            delete mContextMenu;
            mContextMenu = 0;
        }
    }

    //****************************************************************************/
    void QtAssetWidget::addContextMenuAction(const QString& actionText)
    {
        if (mContextMenu)
        {
            mContextMenu->addAction(new QAction(actionText, this));
        }
    }

    //****************************************************************************/
    void QtAssetWidget::showContextMenu(const QPoint& pos)
    {
        if (!mContextMenu)
            return;

        QPoint globalPos = mapToGlobal(pos);
        mContextMenu->popup(globalPos);
    }

    //****************************************************************************/
    void QtAssetWidget::itemSelected(QAction* action)
    {
        // This function is used to intercept the signal annd perform any additional actions (not implemented yet).
        //QMessageBox::information(this, QString("QAction"), QString("item selected" + action->text()));
        emit contextItemSelected(action);
    }

    //****************************************************************************/
    void QtAssetWidget::setContextMenuActionEnabled(const QString& actionText, bool enabled)
    {
        if (!mContextMenu)
            return;

        QList<QAction*> actions = mContextMenu->actions();
        QList<QAction*>::iterator i;
        QAction* action;
        for (i = actions.begin(); i != actions.end(); ++i)
        {
            action = *i;
            if (actionText == action->text())
            {
                action->setEnabled(enabled);
            }
        }
    }

}
