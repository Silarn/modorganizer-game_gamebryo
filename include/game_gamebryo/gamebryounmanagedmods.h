#pragma once

#include <gamefeatures/unmanagedmods.h>

class GameGamebryo;

class GamebryoUnmangedMods : public UnmanagedMods {
  public:
    GamebryoUnmangedMods(const GameGamebryo* game);
    ~GamebryoUnmangedMods();

    virtual QStringList mods(bool onlyOfficial) const override;
    virtual QString displayName(const QString& modName) const override;
    virtual QFileInfo referenceFile(const QString& modName) const override;
    virtual QStringList secondaryFiles(const QString& modName) const override;

  protected:
    const GameGamebryo* game() const { return m_Game; }

  private:
    const GameGamebryo* m_Game;
};
