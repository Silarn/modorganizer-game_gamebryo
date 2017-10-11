#ifndef GAMEBRYOSAVEGAME_H
#define GAMEBRYOSAVEGAME_H

#include <QDateTime>
#include <QFile>
#include <QImage>
#include <QString>
#include <QStringList>
#include <uibase/isavegame.h>

#include <stddef.h>
#include <stdexcept>

struct _SYSTEMTIME;

namespace MOBase {
class IPluginGame;
}

class GamebryoSaveGame : public MOBase::ISaveGame {
  public:
    GamebryoSaveGame(QString const& file, MOBase::IPluginGame const* game);

    virtual ~GamebryoSaveGame();

    virtual QString getFilename() const override;

    virtual QDateTime getCreationTime() const override;

    virtual QString getSaveGroupIdentifier() const override;

    virtual QStringList allFiles() const override;

    // Simple getters
    QString getPCName() const { return m_PCName; }
    unsigned short getPCLevel() const { return m_PCLevel; }
    QString getPCLocation() const { return m_PCLocation; }
    unsigned long getSaveNumber() const { return m_SaveNumber; }
    QStringList const& getPlugins() const { return m_Plugins; }
    QImage const& getScreenshot() const { return m_Screenshot; }

  protected:
    friend class FileWrapper;

    class FileWrapper {
      public:
        /** Construct the save file information.
         * @params expected - expect bytes at start of file
         **/
        FileWrapper(GamebryoSaveGame* game, QString const& expected);

        /** Set this for save games that have a marker at the end of each
         * field. Specifically fallout
         **/
        void setHasFieldMarkers(bool);

        /** Set bz string mode (1 byte length, null terminated)
         **/
        void setBZString(bool);

        template <typename T>
        void skip(int count = 1) {
            if (!m_File.seek(m_File.pos() + count * sizeof(T))) {
                throw std::runtime_error("unexpected end of file");
            }
        }

        template <typename T>
        void read(T& value) {
            int read = m_File.read(reinterpret_cast<char*>(&value), sizeof(T));
            if (read != sizeof(T)) {
                throw std::runtime_error("unexpected end of file");
            }
            if (m_HasFieldMarkers) {
                skip<char>();
            }
        }

        void read(void* buff, std::size_t length);

        /* Reads RGB image from save
         * Assumes picture dimentions come immediately before the save
         */
        void readImage(int scale = 0, bool alpha = false);

        /* Reads RGB image from save */
        void readImage(unsigned long width, unsigned long height, int scale = 0, bool alpha = false);

        /* Read the plugin list */
        void readPlugins(int bytesToIgnore = 0);

        /* Set the creation time from a system date */
        void setCreationTime(::_SYSTEMTIME const&);

      private:
        GamebryoSaveGame* m_Game;
        QFile m_File;
        bool m_HasFieldMarkers;
        bool m_BZString;
    };

    void setCreationTime(_SYSTEMTIME const& time);

    QString m_FileName;
    QString m_PCName;
    unsigned short m_PCLevel;
    QString m_PCLocation;
    unsigned long m_SaveNumber;
    QDateTime m_CreationTime;
    QStringList m_Plugins;
    QImage m_Screenshot;
    MOBase::IPluginGame const* m_Game;
    unsigned short compressionType;
};

template <>
void GamebryoSaveGame::FileWrapper::read<QString>(QString&);

#endif // GAMEBRYOSAVEGAME_H
