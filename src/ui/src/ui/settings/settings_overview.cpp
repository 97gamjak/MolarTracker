#include "ui/settings/settings_overview.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Settings Overview:: Settings Overview object
     *
     * @param title The title to display at the top of the overview
     */
    SettingsOverview::SettingsOverview(const QString& title)
    {
        auto* root = utils::makeQChild<QVBoxLayout>(this);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        // Header
        auto* header       = utils::makeQChild<QWidget>(this);
        auto* headerLayout = utils::makeQChild<QVBoxLayout>(header);

        constexpr std::array<int, 4> margins = {20, 16, 20, 12};
        headerLayout->setContentsMargins(
            margins[0],
            margins[1],
            margins[2],
            margins[3]
        );
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

        auto* cardsContainer = utils::makeQChild<QWidget>(this);
        _cardsLayout         = utils::makeQChild<QVBoxLayout>(cardsContainer);

        constexpr std::array<int, 4> cardMargins = {20, 16, 20, 16};
        constexpr std::size_t        spacing     = 8;
        _cardsLayout->setContentsMargins(
            cardMargins[0],
            cardMargins[1],
            cardMargins[2],
            cardMargins[3]
        );
        _cardsLayout->setSpacing(spacing);
        _cardsLayout->addStretch();

        scroll->setWidget(cardsContainer);
        root->addWidget(scroll, 1);
    }

    /**
     * @brief Add a section card to the overview, this creates a card with the
     * given title and description, and when clicked, it calls the navigation
     * callback with the provided stack index to navigate to the corresponding
     * section in the sidebar/stack.
     *
     * @param title The title to display on the card
     * @param description A brief description to display on the card
     * @param stackIndex The index of the stack page to navigate to when the
     * card is clicked
     */
    void SettingsOverview::addCard(
        const QString& title,
        const QString& description,
        int            stackIndex
    )
    {
        auto* card = utils::makeQChild<QPushButton>(this);
        card->setObjectName("overviewCard");
        card->setCursor(Qt::PointingHandCursor);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        constexpr size_t cardHeight = 64;
        card->setFixedHeight(cardHeight);

        auto* cardLayout = utils::makeQChild<QHBoxLayout>(card);

        constexpr std::array<int, 4> cardMargins = {16, 12, 16, 12};
        constexpr std::size_t        cardSpacing = 12;
        cardLayout->setContentsMargins(
            cardMargins[0],
            cardMargins[1],
            cardMargins[2],
            cardMargins[3]
        );
        cardLayout->setSpacing(cardSpacing);

        auto* textLayout = utils::makeQChild<QVBoxLayout>(card);

        constexpr std::size_t textSpacing = 2;
        textLayout->setSpacing(textSpacing);

        auto* titleLabel = utils::makeQChild<QLabel>(title);
        titleLabel->setObjectName("cardTitle");

        auto* descLabel = utils::makeQChild<QLabel>(description);
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

    /**
     * @brief Set the navigation callback for the overview cards, this callback
     * is called with the stack index to navigate to when a card is clicked,
     * this allows the overview to communicate with the parent dialog to switch
     * to the appropriate section when a card is clicked.
     *
     * @param callback The callback function to call when a card is clicked, it
     * should take an int parameter which is the stack index to navigate to
     */
    void SettingsOverview::setOnNavigate(OnNavigate callback)
    {
        _onNavigate = std::move(callback);
    }

}   // namespace ui