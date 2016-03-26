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

#ifndef __HLMS_EDITOR_PLUGIN_ACTION_H__
#define __HLMS_EDITOR_PLUGIN_ACTION_H__

#include <QAction>
#include "hlms_editor_plugin.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class PluginAction : public QAction
{
    Q_OBJECT

    public:
        // Public plugin
        Ogre::HlmsEditorPlugin* mHlmsEditorPlugin;

        // Constructor
        PluginAction (Ogre::HlmsEditorPlugin* plugin, const QString& text, QObject* parent);

        // Destructor
        virtual ~PluginAction (void);

    signals:
        void pluginActionTriggered (Ogre::HlmsEditorPlugin* plugin);

    private slots:
        void interceptTriggered (bool checked);
};

#endif
