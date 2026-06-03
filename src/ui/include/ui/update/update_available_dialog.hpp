#ifndef __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__
#define __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__

#include "ui/base/dialog.hpp"
#include "utils/version.hpp"

class QCheckBox;   // Forward declaration
class QWidget;     // Forward declaration

namespace ui
{

    /**
     * @brief Dialog shown when a newer version of MolarTracker is available.
     *
     * Displays the available version, a link to the releases page, and a
     * "don't show again for this version" checkbox. The caller should check
     * isDismissedForVersion() after exec() returns to decide whether to
     * persist the dismissal.
     */
    class UpdateAvailableDialog : public Dialog
    {
        Q_OBJECT

       public:
        explicit UpdateAvailableDialog(
            const utils::SemVer& latestVersion,
            QWidget*             parent = nullptr
        );

        [[nodiscard]] bool isDismissedForVersion() const;

       private:
        QCheckBox* _dismissCheckBox{nullptr};
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__UPDATE__UPDATE_AVAILABLE_DIALOG_HPP__
