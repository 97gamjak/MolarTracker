#include "ui/utils/error.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <optional>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    /**
     * @brief Construct a new Error Dialog:: Error Dialog object
     *
     * @param title The title of the error dialog.
     * @param message The error message to display.
     * @param details Optional detailed error message.
     * @param parent The parent widget for the dialog.
     */
    ErrorDialog::ErrorDialog(
        const QString&                title,
        const QString&                message,
        const std::optional<QString>& details,
        QWidget*                      parent
    )
        : Dialog(parent)
    {
        setWindowTitle(title);
        setModal(true);

        auto*          layout  = makeQChild<QVBoxLayout>(this);
        constexpr auto spacing = 16;
        layout->setSpacing(spacing);
        constexpr auto margins = QMargins(20, 20, 20, 16);
        layout->setContentsMargins(margins);

        // Icon + message row
        auto*          topRow   = makeQChild<QHBoxLayout>();
        auto*          icon     = makeQChild<QLabel>(this);
        constexpr auto iconSize = 32;
        icon->setPixmap(
            style()
                ->standardIcon(QStyle::SP_MessageBoxCritical)
                .pixmap(iconSize, iconSize)
        );
        icon->setFixedSize(iconSize, iconSize);

        auto* msgLabel = makeQChild<QLabel>(message, this);
        msgLabel->setWordWrap(true);
        msgLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        topRow->addWidget(icon, 0, Qt::AlignTop);
        constexpr auto topRowSpacing = 12;
        topRow->addSpacing(topRowSpacing);
        topRow->addWidget(msgLabel, 1);
        layout->addLayout(topRow);

        if (details.has_value())
        {
            auto* detailsLabel = makeQChild<QLabel>(*details, this);
            detailsLabel->setWordWrap(true);
            detailsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            layout->addWidget(detailsLabel);
        }

        // OK button
        auto* btnRow = makeQChild<QHBoxLayout>();
        btnRow->addStretch();
        auto* okBtn = new QPushButton("OK", this);
        okBtn->setDefault(true);
        constexpr auto buttonWidth = 80;
        okBtn->setFixedWidth(buttonWidth);
        connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
        btnRow->addWidget(okBtn);
        layout->addLayout(btnRow);
    }

    /**
     * @brief Show the error dialog
     *
     * @param message
     * @param parent
     */
    void ErrorDialog::show(const QString& message, QWidget* parent)
    {
        show("Error", message, parent);
    }

    /**
     * @brief Show the error dialog
     *
     * @param title
     * @param message
     * @param parent
     */
    void ErrorDialog::show(
        const QString& title,
        const QString& message,
        QWidget*       parent
    )
    {
        ErrorDialog dlg(title, message, std::nullopt, parent);
        dlg.exec();
    }

    /**
     * @brief Show the error dialog
     *
     * @param message
     * @param parent
     */
    void ErrorDialog::show(const std::string& message, QWidget* parent)
    {
        show(QString::fromStdString(message), parent);
    }

    /**
     * @brief Show the error dialog
     *
     * @param title
     * @param message
     * @param parent
     */
    void ErrorDialog::show(
        const std::string& title,
        const std::string& message,
        QWidget*           parent
    )
    {
        show(
            QString::fromStdString(title),
            QString::fromStdString(message),
            parent
        );
    }
}   // namespace ui