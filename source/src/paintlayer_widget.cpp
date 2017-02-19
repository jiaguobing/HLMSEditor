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
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QEvent>
#include <QPixmap>
#include <QImage>
#include <QMimeData>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include <QVector2D>
#include "constants.h"
#include "paintlayer_widget.h"
#include "paintlayer_dockwidget.h"
#include "paintlayer_dialog.h"

//****************************************************************************/
PaintLayerWidget::PaintLayerWidget(const QString& iconDir, PaintLayerDockWidget* paintLayerDockWidget, QWidget* parent) :
    QWidget(parent),
    mPaintLayerDockWidget(paintLayerDockWidget)
{
    setWindowTitle(QString("Layers"));
    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout* tableLayout = new QVBoxLayout;
    mIconDir = iconDir;
    mLayerIdCounter = 1;
    mListenToSceneId = 0;
    mListenToDeleteEvents = true;
    helperIntVector.clear();

    // Create table
    mTable = new QTableWidget(0, 4, this);
    mTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTable->setAcceptDrops(false);
    mTable->setShowGrid(false);
    mTable->viewport()->installEventFilter(this);
    QRect rect = QApplication::desktop()->screenGeometry();
    mTable->setColumnWidth(TOOL_LAYER_COLUMN_ICON, 2 * TOOL_LAYER_ICON_WIDTH);
    mTable->setColumnWidth(TOOL_LAYER_COLUMN_NAME, TOOL_LAYER_NAME_WIDTH);
    mTable->setColumnWidth(TOOL_LAYER_COLUMN_VISIBILITY, 2 * TOOL_LAYER_ICON_WIDTH);
    mTable->setColumnWidth(TOOL_LAYER_COLUMN_FILLER, rect.width());
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    setStyleSheet("QLineEdit{padding: 0 0px; margin-left: 0px; margin-right: 0px; max-height: 28px; height: 28px;}");
    connect(mTable, SIGNAL(clicked(QModelIndex)), this, SLOT(tableClicked(QModelIndex)));
    connect(mTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(handleTableDoubleClicked(QModelIndex)));

    // Set headers
    QStringList headers;
    headers << tr("Edit") << tr("Layer name") << tr("Visibility") << tr("");
    mTable->setHorizontalHeaderLabels(headers);
    QFont font;
    font.setBold(true);
    mTable->horizontalHeader()->setFont(font);
    mTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    // Contextmenu
    setContextMenuPolicy(Qt::CustomContextMenu);
    mContextMenu = new QMenu(mTable);
    mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_CREATE_LAYER, mTable));
    mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_DELETE_LAYER, mTable));
    mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_RENAME_LAYER, mTable));
    mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_ALL_VISIBLE, mTable));
    //mContextMenu->addAction(new QAction(QString("TEST"), mTable));
    connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuItemSelected(QAction*)));

    // Layout
    tableLayout->addWidget(mTable);
    mainLayout->addLayout(tableLayout);
    setLayout(mainLayout);
}

//****************************************************************************/
PaintLayerWidget::~PaintLayerWidget(void)
{
    QVector<QtLayer*>::iterator it;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        delete *it;

    mLayerVec.clear(); // The table is automatically deleted when this widget is deleted
}

//****************************************************************************/
bool PaintLayerWidget::eventFilter(QObject* object, QEvent* event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*) event;
    switch ((int) event->type())
    {
        case QEvent::MouseButtonPress:
            mouseClickHandler(mouseEvent);
        break;

        case QEvent::MouseButtonDblClick:
            mouseDblClickHandler(mouseEvent);
        break;

        break;
    }
    return QObject::eventFilter(object, event);
}

//****************************************************************************/
void PaintLayerWidget::tableClicked(QModelIndex index)
{
    // Toggle visibility
    int row = index.row();
    int col = index.column();
    QtLayer* layer = mLayerVec.at(row);
    if (col == TOOL_LAYER_COLUMN_VISIBILITY)
    {
        if (layer->visible == true)
            updateVisibilityIcon(row, false);
        else
            updateVisibilityIcon(row, true);
    }

    // Signal that layer is selected
    emit layerSelected(layer->layerId, layer->name);
}

//****************************************************************************/
QVector<int> PaintLayerWidget::getSelectedLayerIds(void)
{
    helperIntVector.clear();
    QList<QTableWidgetItem*> itemList = mTable->selectedItems();
    foreach(QTableWidgetItem* item, itemList)
    {
        // Do not append all items, but only the rows; so only mark in case of a distinct column
        if (item->column() == TOOL_LAYER_COLUMN_ICON)
            helperIntVector.append(item->data(Qt::UserRole).toInt());
    }

    return helperIntVector;
}

//****************************************************************************/
void PaintLayerWidget::mouseClickHandler(QMouseEvent* event)
{
    switch ((int) event->button())
    {
        case Qt::LeftButton:
        {
            // TODO
        }
        break;

        case Qt::RightButton:
        {
            QPoint pos;
            pos.setX(event->screenPos().x());
            pos.setY(event->screenPos().y());
            mContextMenu->popup(pos);
        }
        break;
    }
}

//****************************************************************************/
void PaintLayerWidget::mouseDblClickHandler(QMouseEvent* event)
{
    switch ((int) event->button())
    {
        case Qt::LeftButton:
        {
            if (mTable->currentColumn() == TOOL_LAYER_COLUMN_NAME)
            {
                QTableWidgetItem* item = mTable->currentItem();
                if (item->column() == TOOL_LAYER_COLUMN_NAME)
                {
                    // Its the name; edit it
                    mTable->editItem(item);
                }
            }
        }
        break;
    }
}

//****************************************************************************/
void PaintLayerWidget::contextMenuItemSelected(QAction* action)
{
    // ---------------- Create layer action ----------------
    if (action->text() == TOOL_LAYER_ACTION_CREATE_LAYER)
    {
        createDefaultPaintLayer();
        return;
    }
    else if (action->text() == TOOL_LAYER_ACTION_DELETE_LAYER)
    {
        deleteSelectedPaintLayer();
    }
    else if (action->text() == TOOL_LAYER_ACTION_RENAME_LAYER)
    {
        QTableWidgetItem* item = mTable->currentItem();
        if (item && item->column() == TOOL_LAYER_COLUMN_NAME)
        {
            // Its the name; edit it
            mTable->editItem(item);
        }
    }
    else if (action->text() == TOOL_LAYER_ACTION_ALL_VISIBLE)
    {
        int rows = mTable->rowCount();
        for(int row = 0; row < rows; ++row)
            if (mLayerVec.at(row)->visible == false)
                updateVisibilityIcon(row, true);
    }
    else if (action->text() == QString ("TEST"))
    {
        //setLayerName(3, QString("Test"));
        //deleteLayer(QString("New layer 4"));
    }
}

//****************************************************************************/
void PaintLayerWidget::updateVisibilityIcon(int row, bool visible)
{
    QTableWidgetItem* item = mTable->item(row, TOOL_LAYER_COLUMN_VISIBILITY);
    if (!item)
        return;

    QPixmap pixMap;
    QtLayer* layer = mLayerVec.at(row);
    layer->visible = visible;
    if (visible)
    {
        QImage image(mIconDir + TOOL_ICON_VIEW_VISIBLE);
        pixMap = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
        item->setData(Qt::DecorationRole, QVariant(pixMap));
        //layer->visible = true;
    }
    else
    {
        QImage image(mIconDir + TOOL_ICON_VIEW_INVISIBLE);
        pixMap = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
        item->setData(Qt::DecorationRole, QVariant(pixMap));
        //layer->visible = false;
    }

    emit layerVisibiltyChanged(layer->layerId, item->text(), layer->visible);
}

//****************************************************************************/
int PaintLayerWidget::newLayerId(void)
{
    int maxId = 0;
    QtLayer* layer;
    QVector<QtLayer*>::iterator it;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        layer = *it;
        if (layer->layerId > maxId)
            maxId = layer->layerId;
    }

    ++maxId;
    return maxId;
}

//****************************************************************************/
void PaintLayerWidget::addLayer(QtLayer* layer)
{
    // Add the layer to the vector create a new row in the table
    int row = mTable->rowCount();
    mLayerVec.append(layer);
    mTable->insertRow(row);

    // Set the default brush icon
    QTableWidgetItem* item = new QTableWidgetItem();
    QImage image(DEFAULT_BRUSH_AND_PATH_QSTRING);
    QPixmap pixMapLayer = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
    item->setData(Qt::DecorationRole, QVariant(pixMapLayer));
    item->setData(Qt::UserRole, QVariant(layer->layerId));
    mTable->setItem(row, TOOL_LAYER_COLUMN_ICON, item);

    // Set the name
    item = new QTableWidgetItem();
    QString newName = layer->name;
    if (newName.size() == 0)
        newName = QString("Paint layer ") + QVariant(layer->layerId).toString();
    layer->name = newName;
    item->setText(newName);
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setData(Qt::UserRole, QVariant(layer->layerId));
    mTable->setItem(row, TOOL_LAYER_COLUMN_NAME, item);

    // Set the visibility icon
    item = new QTableWidgetItem();
    QImage imageVis(mIconDir + TOOL_ICON_VIEW_VISIBLE);
    QPixmap pixMapVis = QPixmap::fromImage(imageVis).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
    item->setData(Qt::DecorationRole, QVariant(pixMapVis));
    item->setData(Qt::UserRole, QVariant(layer->layerId));
    mTable->setItem(row, TOOL_LAYER_COLUMN_VISIBILITY, item);

    // Set the layerId in the visibility icon
    //item->setData(Qt::UserRole, QVariant(layer->layerId));

    //mTable->resizeColumnToContents(0);

    // Set the item to selected
    mTable->selectRow(row);
    mTable->resizeRowsToContents();

    // Emit signal
    emit layerCreatedOrAdded(layer->layerId, layer->name);
}

//****************************************************************************/
void PaintLayerWidget::createDefaultPaintLayer (void)
{
    if (mPaintLayerDockWidget->currentDatablockExists())
    {
        if (mPaintLayerDockWidget->currentDatablockIsPbs())
        {
            createLayer(QString(""));
        }
        else
        {
            QMessageBox::information(0, QString("Warning"), QString("Painting on HLMS Unlit materials is not supported"));
        }
    }
    else
    {
        QMessageBox::information(0, QString("Warning"), QString("Create / load a hlms material before creating a paint layer.\n"
                                                                "Did you forget to generate (cog icon) or edit the hlms material?"));
    }
}

//****************************************************************************/
const QtLayer* PaintLayerWidget::createLayer(const QString& name)
{
    // Fill the layer object
    QtLayer* layer = new QtLayer();
    layer->layerId = newLayerId();
    layer->name = name;
    layer->visible = true;

    // Add the layer to the layer vector and the table
    addLayer(layer);
    return layer;
}


//****************************************************************************/
const QtLayer* PaintLayerWidget::createLayer(int layerId, const QString& name)
{
    // Fill the layer object
    QtLayer* layer = new QtLayer();
    layer->layerId = layerId;
    layer->name = name;
    layer->visible = true;

    // Add the layer to the layer vector and the table
    addLayer(layer);
    return layer;
}

//****************************************************************************/
int PaintLayerWidget::getRow(const QString& name)
{
    int rows = mTable->rowCount();
    QTableWidgetItem* item;
    for (int row = 0; row < rows; ++row)
    {
        item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
        if (item->text() == name)
            return row;
    }

    return -1; // Not found
}

//****************************************************************************/
int PaintLayerWidget::getRow(int layerId)
{
    int row = 0;
    QtLayer* layer;
    QVector<QtLayer*>::iterator it;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        layer = *it;
        if (layer->layerId == layerId)
            return row;

        ++row;
    }

    return -1; // Not found
}

//****************************************************************************/
void PaintLayerWidget::deleteLayer (const QString& name)
{
    int row = getRow(name);
    if (row != -1)
    {
        // Delete the layer from the table and the vector
        QString name = getName(row);
        mTable->removeRow(row);
        int layerId = removeFromLayerVec(row);

        emit layerDeleted(layerId, name);
    }
}

//****************************************************************************/
void PaintLayerWidget::deleteLayer (const QtLayer* layer)
{
    // Delete the layer
    int row = 0;
    QVector<QtLayer*>::iterator it;
    int layerId;
    QString name;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        if (layer == *it)
        {
            // Delete the layer from the table and the vector
            layerId = layer->layerId;
            mLayerVec.erase(it);
            name = getName(row);
            mTable->removeRow(row);
            delete layer;

            emit layerDeleted(layerId, name);
        }
        ++row;
    }
}

//****************************************************************************/
void PaintLayerWidget::deleteLayer (int layerId)
{
    int row = getRow(layerId);
    if (row != -1)
    {
        // Delete the layer from the table and the vector
        QString name = getName(row);
        mTable->removeRow(row);
        int layerId = removeFromLayerVec(row);

        emit layerDeleted(layerId, name);
    }
}

//****************************************************************************/
void PaintLayerWidget::deleteAllLayers (void)
{
    mTable->clearContents();
    mTable->setRowCount(0);
    mLayerVec.clear();
}

//****************************************************************************/
void PaintLayerWidget::deleteSelectedPaintLayer(void)
{
    int layerId = getCurrentLayerId();
    if (layerId < 0)
        QMessageBox::information(0, QString("Warning"), QString("There is no layer to delete"));

    deleteLayer(getCurrentLayerId());
}

//****************************************************************************/
int PaintLayerWidget::removeFromLayerVec(int row)
{
    int layerId;
    QtLayer* layer = mLayerVec.at(row);
    layerId = layer->layerId;
    mLayerVec.removeAt(row);
    delete layer;
    return layerId;
}

//****************************************************************************/
void PaintLayerWidget::setLayerName(int layerId, const QString& newName)
{
    int row = getRow(layerId);
    if (row != -1)
    {
        QTableWidgetItem* item;
        item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
        item->setText(newName);
    }
}

//****************************************************************************/
QtLayer* PaintLayerWidget::getLayer(const QString& name)
{
    int row = 0;
    QVector<QtLayer*>::iterator it;
    QtLayer* layer;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        layer = *it;
        if (name == getName(row))
        {
            layer->name = name; // Explicitly set the name, because it is not automatically in sync with the table
            return layer;
        }

        ++row;
    }

    return 0;
}

//****************************************************************************/
QtLayer* PaintLayerWidget::getLayer(int layerId)
{
    QVector<QtLayer*>::iterator it;
    QtLayer* layer;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        layer = *it;
        if (layer->layerId = layerId)
            return layer;
    }

    return 0;
}

//****************************************************************************/
QVector<QtLayer*> PaintLayerWidget::getLayers(void)
{
    // Sync the name in the table with the vector
    int row = 0;
    QVector<QtLayer*>::iterator it;
    QtLayer* layer;
    for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
    {
        layer = *it;
        layer->name = getName(row); // Explicitly set the name, because it is not automatically in sync with the table
        ++row;
    }

    return mLayerVec;
}

//****************************************************************************/
const QString& PaintLayerWidget::getName(int row)
{
    QTableWidgetItem* item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
    if (item)
        mTempName = item->text();

    return mTempName;
}

//****************************************************************************/
int PaintLayerWidget::getCurrentLayerId(void)
{
    int currentRow = mTable->currentRow();
    if (currentRow < 0)
        return -1;

    QtLayer* layer = mLayerVec[currentRow];
    if (layer)
        return layer->layerId;

    if (mLayerVec.count() > 0)
        return 0;

    return -1;
}

//****************************************************************************/
void PaintLayerWidget::handleTableDoubleClicked(QModelIndex index)
{
    int row = index.row();
    int col = index.column();
    QtLayer* layer = mLayerVec.at(row);
    int layerId = layer->layerId;
    if (col == TOOL_LAYER_COLUMN_ICON)
    {
        PaintLayerDialog paintLayerDialog (this, layer);
        paintLayerDialog.setMinimumWidth(600);
        paintLayerDialog.setMinimumHeight(800);
        initialisePaintLayerDialog(&paintLayerDialog, layerId); // Fill the dialog with values from the associated PaintLayer
        paintLayerDialog.exec();

        // Don't check whether the values are changed; just set them in the layer (by means of the mPaintLayerDockWidget as intermediate)
        mPaintLayerDockWidget->setTextureType(layerId, paintLayerDialog.mTextureTypeSelectProperty->getCurrentText());
        mPaintLayerDockWidget->setPaintEffect(layerId, paintLayerDialog.mPaintEffectSelectProperty->getCurrentText());
        mPaintLayerDockWidget->setPaintOverflow(layerId, paintLayerDialog.mPaintOverflowSelectProperty->getCurrentText());

        // Paint Colour
        if (paintLayerDialog.mPaintJitterCheckboxProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterPaintColourMin(layerId, paintLayerDialog.mPaintColourJitterMinProperty->getColor());
            mPaintLayerDockWidget->setJitterPaintColourMax(layerId, paintLayerDialog.mPaintColourJitterMaxProperty->getColor());
            mPaintLayerDockWidget->setJitterPaintColourInterval(layerId, paintLayerDialog.mPaintColourJitterIntervalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setPaintColour(layerId, paintLayerDialog.mPaintColourProperty->getColor());
        }

        // Force
        if (paintLayerDialog.mJitterForceCheckboxProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterForceMin(layerId, paintLayerDialog.mJitterForceMinProperty->getValue());
            mPaintLayerDockWidget->setJitterForceMax(layerId, paintLayerDialog.mJitterForceMaxProperty->getValue());
            mPaintLayerDockWidget->setJitterForceInterval(layerId, paintLayerDialog.mJitterForceIntervalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setBrushForce(layerId, paintLayerDialog.mForceProperty->getValue());
        }


        // Scale
        if (paintLayerDialog.mJitterScaleCheckboxProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterScaleMin(layerId, paintLayerDialog.mJitterScaleMinProperty->getValue());
            mPaintLayerDockWidget->setJitterScaleMax(layerId, paintLayerDialog.mJitterScaleMaxProperty->getValue());
            mPaintLayerDockWidget->setJitterScaleInterval(layerId, paintLayerDialog.mJitterScaleIntervalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setBrushScale(layerId, paintLayerDialog.mScaleProperty->getValue());
        }

        // Rotation angle
        if (paintLayerDialog.mJitterRotationAngleCheckboxProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterRotationAngleMin(layerId, paintLayerDialog.mJitterRotationAngleMinProperty->getValue());
            mPaintLayerDockWidget->setJitterRotationAngleMax(layerId, paintLayerDialog.mJitterRotationAngleMaxProperty->getValue());
            mPaintLayerDockWidget->setJitterRotationAngleInterval(layerId, paintLayerDialog.mJitterRotationAngleIntervalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setRotationAngle(layerId, paintLayerDialog.mRotationAngleProperty->getValue());
        }

        // Translation
        if (paintLayerDialog.mJitterTranslationCheckboxProperty->getValue())
        {
            mHelperVector2D.setX(paintLayerDialog.mJitterTranslationXMinProperty->getValue());
            mHelperVector2D.setY(paintLayerDialog.mJitterTranslationXMaxProperty->getValue());
            mPaintLayerDockWidget->setJitterTranslationX(layerId, mHelperVector2D);
            mHelperVector2D.setX(paintLayerDialog.mJitterTranslationYMinProperty->getValue());
            mHelperVector2D.setY(paintLayerDialog.mJitterTranslationYMaxProperty->getValue());
            mPaintLayerDockWidget->setJitterTranslationY(layerId, mHelperVector2D);
            mPaintLayerDockWidget->setJitterTranslationInterval(layerId, paintLayerDialog.mJitterTranslationIntervalProperty->getValue());
        }
        else
        {
            mHelperVector2D.setX(paintLayerDialog.mTranslationXProperty->getValue());
            mHelperVector2D.setY(paintLayerDialog.mTranslationYProperty->getValue());
            mPaintLayerDockWidget->setTranslation(layerId, mHelperVector2D);
        }

        // Mirror Horizontal
        if (paintLayerDialog.mJitterMirrorHorizontalProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterMirrorHorizontalInterval(layerId, paintLayerDialog.mJitterMirrorHorizontalIntervalProperty->getValue());
            mPaintLayerDockWidget->setJitterMirrorHorizontal(layerId, paintLayerDialog.mJitterMirrorHorizontalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setMirrorHorizontal(layerId, paintLayerDialog.mMirrorHorizontalProperty->getValue());
        }

        // Mirror Vertical
        if (paintLayerDialog.mJitterMirrorVerticalProperty->getValue())
        {
            mPaintLayerDockWidget->setJitterMirrorVerticalInterval(layerId, paintLayerDialog.mJitterMirrorVerticalIntervalProperty->getValue());
            mPaintLayerDockWidget->setJitterMirrorVertical(layerId, paintLayerDialog.mJitterMirrorVerticalProperty->getValue());
        }
        else
        {
            mPaintLayerDockWidget->setMirrorVertical(layerId, paintLayerDialog.mMirrorVerticalProperty->getValue());
        }
    }
}

//****************************************************************************/
void PaintLayerWidget::initialisePaintLayerDialog(PaintLayerDialog* paintLayerDialog, int layerId)
{
    // General
    paintLayerDialog->mTextureTypeSelectProperty->setCurrentText(mPaintLayerDockWidget->getTextureType(layerId));
    paintLayerDialog->mPaintEffectSelectProperty->setCurrentText(mPaintLayerDockWidget->getPaintEffect(layerId));
    paintLayerDialog->mPaintOverflowSelectProperty->setCurrentText(mPaintLayerDockWidget->getPaintOverflow(layerId));

    // Paint Colour
    paintLayerDialog->mPaintJitterCheckboxProperty->setValue(mPaintLayerDockWidget->hasJitterPaintEnabled(layerId));
    paintLayerDialog->mPaintColourProperty->setColor(mPaintLayerDockWidget->getPaintColour(layerId));
    paintLayerDialog->mPaintColourJitterMinProperty->setColor(mPaintLayerDockWidget->getJitterPaintColourMin(layerId));
    paintLayerDialog->mPaintColourJitterMaxProperty->setColor(mPaintLayerDockWidget->getJitterPaintColourMax(layerId));
    paintLayerDialog->mPaintColourJitterIntervalProperty->setValue(mPaintLayerDockWidget->getJitterPaintColourInterval(layerId));

    // Force
    paintLayerDialog->mJitterForceCheckboxProperty->setValue(mPaintLayerDockWidget->hasJitterForceEnabled(layerId));
    paintLayerDialog->mForceProperty->setValue(mPaintLayerDockWidget->getBrushForce(layerId));
    paintLayerDialog->mJitterForceMinProperty->setValue(mPaintLayerDockWidget->getJitterForceMin(layerId));
    paintLayerDialog->mJitterForceMaxProperty->setValue(mPaintLayerDockWidget->getJitterForceMax(layerId));
    paintLayerDialog->mJitterForceIntervalProperty->setValue(mPaintLayerDockWidget->getJitterForceInterval(layerId));

    // Scale
    paintLayerDialog->mJitterScaleCheckboxProperty->setValue(mPaintLayerDockWidget->hasJitterScaleEnabled(layerId));
    paintLayerDialog->mScaleProperty->setValue(mPaintLayerDockWidget->getBrushScale(layerId));
    paintLayerDialog->mJitterScaleMinProperty->setValue(mPaintLayerDockWidget->getJitterScaleMin(layerId));
    paintLayerDialog->mJitterScaleMaxProperty->setValue(mPaintLayerDockWidget->getJitterScaleMax(layerId));
    paintLayerDialog->mJitterScaleIntervalProperty->setValue(mPaintLayerDockWidget->getJitterScaleInterval(layerId));

    // Rotation angle
    paintLayerDialog->mJitterRotationAngleCheckboxProperty->setValue(mPaintLayerDockWidget->hasJitterRotationAngleEnabled(layerId));
    paintLayerDialog->mRotationAngleProperty->setValue(mPaintLayerDockWidget->getRotationAngle(layerId));
    paintLayerDialog->mJitterRotationAngleMinProperty->setValue(mPaintLayerDockWidget->getJitterRotationAngleMin(layerId));
    paintLayerDialog->mJitterRotationAngleMaxProperty->setValue(mPaintLayerDockWidget->getJitterRotationAngleMax(layerId));
    paintLayerDialog->mJitterRotationAngleIntervalProperty->setValue(mPaintLayerDockWidget->getJitterRotationAngleInterval(layerId));

    // Translation
    paintLayerDialog->mJitterTranslationCheckboxProperty->setValue(mPaintLayerDockWidget->hasJitterTranslationEnabled(layerId));
    paintLayerDialog->mTranslationXProperty->setValue(mPaintLayerDockWidget->getTranslation(layerId).x());
    paintLayerDialog->mTranslationYProperty->setValue(mPaintLayerDockWidget->getTranslation(layerId).y());
    paintLayerDialog->mJitterTranslationXMinProperty->setValue(mPaintLayerDockWidget->getJitterTranslationX(layerId).x());
    paintLayerDialog->mJitterTranslationXMaxProperty->setValue(mPaintLayerDockWidget->getJitterTranslationX(layerId).y());
    paintLayerDialog->mJitterTranslationYMinProperty->setValue(mPaintLayerDockWidget->getJitterTranslationY(layerId).x());
    paintLayerDialog->mJitterTranslationYMaxProperty->setValue(mPaintLayerDockWidget->getJitterTranslationY(layerId).y());
    paintLayerDialog->mJitterTranslationIntervalProperty->setValue(mPaintLayerDockWidget->getJitterTranslationInterval(layerId));

    // Mirror Horizontal
    paintLayerDialog->mJitterMirrorHorizontalProperty->setValue(mPaintLayerDockWidget->hasJitterMirrorHorizontal(layerId));
    paintLayerDialog->mMirrorHorizontalProperty->setValue(mPaintLayerDockWidget->getMirrorHorizontal(layerId));
    paintLayerDialog->mJitterMirrorHorizontalIntervalProperty->setValue(mPaintLayerDockWidget->getJitterMirrorHorizontalInterval(layerId));

    // Mirror Vertical
    paintLayerDialog->mJitterMirrorVerticalProperty->setValue(mPaintLayerDockWidget->hasJitterMirrorVertical(layerId));
    paintLayerDialog->mMirrorVerticalProperty->setValue(mPaintLayerDockWidget->getMirrorVertical(layerId));
    paintLayerDialog->mJitterMirrorVerticalIntervalProperty->setValue(mPaintLayerDockWidget->getJitterMirrorVerticalInterval(layerId));
}

//****************************************************************************/
QStringList PaintLayerWidget::getAvailableTextureTypes(void)
{
    return mPaintLayerDockWidget->getAvailableTextureTypes();
}

//****************************************************************************/
void PaintLayerWidget::setBrushIconInCurrentLayers (const QString& brushFileName)
{
    QImage image(brushFileName);
    QPixmap pixMapLayer = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
    QList<QTableWidgetItem*> itemList = mTable->selectedItems();
    foreach(QTableWidgetItem* item, itemList)
    {
        if (item->column() == TOOL_LAYER_COLUMN_ICON)
        {
            item->setData(Qt::DecorationRole, QVariant(pixMapLayer));
        }
    }
}
