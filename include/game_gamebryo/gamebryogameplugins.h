#pragma once

#include <QDateTime>
#include <QTextCodec>
#include <gamefeatures/gameplugins.h>
#include <uibase/imoinfo.h>

class GamebryoGamePlugins : public GamePlugins {
  public:
    GamebryoGamePlugins(MOBase::IOrganizer* organizer);

    virtual void writePluginLists(const MOBase::IPluginList* pluginList) override;
    virtual void readPluginLists(MOBase::IPluginList* pluginList) override;

  protected:
    QTextCodec* utf8Codec() const { return m_Utf8Codec; }
    QTextCodec* localCodec() const { return m_LocalCodec; }

    MOBase::IOrganizer* organizer() const { return m_Organizer; }

    virtual void writePluginList(const MOBase::IPluginList* pluginList, const QString& filePath);
    virtual void writeLoadOrderList(const MOBase::IPluginList* pluginList, const QString& filePath);
    virtual bool readLoadOrderList(MOBase::IPluginList* pluginList, const QString& filePath);
    virtual bool readPluginList(MOBase::IPluginList* pluginList, const QString& filePath, bool useLoadOrder);

  private:
    void writeList(const MOBase::IPluginList* pluginList, const QString& filePath, bool loadOrder);

  private:
    MOBase::IOrganizer* m_Organizer;
    QTextCodec* m_Utf8Codec;
    QTextCodec* m_LocalCodec;

    QDateTime m_LastRead;
    std::map<QString, QByteArray> m_LastSaveHash;
};
