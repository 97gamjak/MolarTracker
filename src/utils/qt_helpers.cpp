#include "qt_helpers.hpp"

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QString>
#include <QWidget>
#include <string>

namespace utils
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

}   // namespace utils