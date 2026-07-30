#include "common/qt_helpers.hpp"

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QString>
#include <QWidget>
#include <string>

#include "common/shortcut_set.hpp"

namespace common
{
    /**
     * @brief Converts a span of string_views to a QStringList
     *
     * @param vec
     * @return QStringList
     */
    QStringList toQStringList(const std::span<const std::string_view>& vec)
    {
        QStringList list;
        for (const auto& str : vec)
            list.append(QString::fromStdString(std::string{str}));

        return list;
    }

    QStringList toQStringList(const std::span<std::string>& vec)
    {
        QStringList list;
        for (const auto& str : vec)
            list.append(QString::fromStdString(str));

        return list;
    }

    /**
     * @brief Converts a span of strings to a vector of QStrings
     *
     * @param vec The input span of strings
     * @return std::vector<QString> The converted vector of QStrings
     */
    std::vector<QString> toQStringVector(const std::span<std::string>& vec)
    {
        std::vector<QString> list;
        list.reserve(vec.size());
        for (const auto& str : vec)
            list.push_back(QString::fromStdString(str));
        return list;
    }

    /**
     * @brief Converts a span of strings to a set of QStrings
     *
     * @param vec The input span of strings
     * @return Set<QString> The converted set of QStrings
     */
    Set<QString> toQStringSet(const Set<std::string>& vec)
    {
        Set<QString> set;
        for (const auto& str : vec)
            set.insert(QString::fromStdString(str));
        return set;
    }

    /**
     * @brief Converts a span of const strings to a vector of QStrings
     *
     * @param vec The input span of const strings
     * @return std::vector<QString> The converted vector of QStrings
     */
    std::vector<QString> toQStringVector(
        const std::span<const std::string>& vec
    )
    {
        std::vector<QString> list;
        list.reserve(vec.size());
        for (const auto& str : vec)
            list.push_back(QString::fromStdString(str));
        return list;
    }

    /**
     * @brief Moves the dialog to the center of the parent's screen
     *
     * @param dlg The dialog to move
     * @param parent The parent widget
     */
    void moveDialogToParentScreenCenter(QDialog* dlg, QWidget* parent)
    {
        if (dlg == nullptr || parent == nullptr)
            return;

        QScreen* screen = parent->screen();
        QPoint   center = screen->availableGeometry().center();

        center.setX(center.x() - (dlg->width() / 2));
        center.setY(center.y() - (dlg->height() / 2));
        dlg->move(center);
    }

    /**
     * @brief Converts a size in bytes to a human-readable string in KB
     *
     * @param bytes The size in bytes
     * @return QString The human-readable string in KB
     */
    QString toKBString(std::size_t bytes)
    {
        static constexpr auto byteConversion = 1024;
        const auto kiloBytes = static_cast<double>(bytes) / byteConversion;
        return QString{"%1 KB"}.arg(kiloBytes);
    }

    /**
     * @brief Sets the keyboard shortcut for a QAction based on a ShortcutSet
     *
     * @param action The QAction to set the shortcut for
     * @param shortcutSet The ShortcutSet containing the shortcut information
     */
    void setShortcut(QAction* action, const ShortcutSet& shortcutSet)
    {
        if (action == nullptr)
            return;

        const auto& shortcuts = shortcutSet.getShortcuts();
        if (shortcuts.empty())
            return;

        action->setShortcuts(shortcutSet.toQKeySequences());

        switch (shortcutSet.getContext())
        {
            case ShortcutContext::Global:
                action->setShortcutContext(Qt::ApplicationShortcut);
                break;
            case ShortcutContext::Window:
                action->setShortcutContext(Qt::WindowShortcut);
                break;
            case ShortcutContext::Default:
                break;
        }
    }

}   // namespace common