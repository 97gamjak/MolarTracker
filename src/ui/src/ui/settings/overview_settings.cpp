#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "ui/settings/settings_overview.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    SettingsOverview::SettingsOverview(const QString& title)
    {
        auto* root = new QVBoxLayout(this);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        // Header
        auto* header       = utils::makeQChild<QWidget>(this);
        auto* headerLayout = new QVBoxLayout(header);
        headerLayout->setContentsMargins(20, 16, 20, 12);
        headerLayout->setSpacing(2);
        header->setObjectName("sectionHeader");

        auto* titleLabel = utils::makeQChild<QLabel>(header);
        titleLabel->setText(title);
        titleLabel->setObjectName("sectionTitle");
        headerLayout->addWidget(titleLabel);

        root->addWidget(header);

        auto* divider = utils::makeQChild<QFrame>(this);
        divider->setFrameShape(QFrame::HLine);
        divider->setObjectName("sectionDivider");
        root->addWidget(divider);

        // Scrollable cards area
        auto* scroll = utils::makeQChild<QScrollArea>(this);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        auto* cardsContainer = new QWidget();
        _cardsLayout         = new QVBoxLayout(cardsContainer);
        _cardsLayout->setContentsMargins(20, 16, 20, 16);
        _cardsLayout->setSpacing(8);
        _cardsLayout->addStretch();

        scroll->setWidget(cardsContainer);
        root->addWidget(scroll, 1);
    }

    void SettingsOverview::addCard(
        const QString& title,
        const QString& description,
        int            stackIndex
    )
    {
        auto* card = new QPushButton();
        card->setObjectName("overviewCard");
        card->setCursor(Qt::PointingHandCursor);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        card->setFixedHeight(64);

        auto* cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(16, 12, 16, 12);
        cardLayout->setSpacing(12);

        auto* textLayout = new QVBoxLayout();
        textLayout->setSpacing(2);

        auto* titleLabel = new QLabel(title);
        titleLabel->setObjectName("cardTitle");

        auto* descLabel = new QLabel(description);
        descLabel->setObjectName("cardDesc");
        descLabel->setWordWrap(true);

        textLayout->addWidget(titleLabel);
        textLayout->addWidget(descLabel);
        cardLayout->addLayout(textLayout, 1);

        auto* arrow = new QLabel("→");
        arrow->setObjectName("cardArrow");
        cardLayout->addWidget(arrow);

        // Insert before the trailing stretch
        _cardsLayout->insertWidget(_cardsLayout->count() - 1, card);

        connect(
            card,
            &QPushButton::clicked,
            this,
            [this, stackIndex]()
            {
                if (_onNavigate)
                    _onNavigate(stackIndex);
            }
        );
    }

    void SettingsOverview::setOnNavigate(OnNavigate callback)
    {
        _onNavigate = std::move(callback);
    }

}   // namespace ui