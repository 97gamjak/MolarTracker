#include "ui/position/position_selection_dialog.hpp"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include "drafts/position_draft.hpp"
#include "ui/position/position_selection_table_model.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    /**
     * @brief Construct a new Position Selection Dialog:: Position Selection
     * Dialog object
     *
     * @param positions The list of position drafts to select from
     * @param parent The parent widget
     */
    PositionSelectionDialog::PositionSelectionDialog(
        const std::vector<drafts::PositionStockDetailDraft>& positions,
        QWidget*                                             parent
    )
        : Dialog{parent},
          _model{makeQChild<PositionSelectionTableModel>(positions, this)},
          _tableView{makeQChild<QTableView>(this)},
          _selectBtn{makeQChild<QPushButton>(QStringLiteral("Select"), this)},
          _createBtn{
              makeQChild<QPushButton>(QStringLiteral("Create new"), this)
          },
          _cancelBtn{makeQChild<QPushButton>(QStringLiteral("Cancel"), this)}
    {
        setWindowTitle(QStringLiteral("Position drafts"));

        // Table setup
        _tableView->setModel(_model);
        _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _tableView->setAlternatingRowColors(true);
        _tableView->verticalHeader()->hide();
        _tableView->horizontalHeader()->setStretchLastSection(false);
        _tableView->horizontalHeader()->setSectionResizeMode(
            static_cast<int>(PositionSelectionColumns::Name),
            QHeaderView::Stretch
        );

        // Buttons
        _selectBtn->setDefault(true);
        _selectBtn->setEnabled(false);

        auto* buttonLayout = makeQChild<QHBoxLayout>();
        buttonLayout->addWidget(_createBtn);
        buttonLayout->addStretch();
        buttonLayout->addWidget(_cancelBtn);
        buttonLayout->addWidget(_selectBtn);

        auto* layout = makeQChild<QVBoxLayout>(this);
        layout->addWidget(
            makeQChild<QLabel>(
                QStringLiteral("Select an existing draft or create a new one."),
                this
            )
        );
        constexpr auto spacing = 8;
        layout->addSpacing(spacing);
        layout->addWidget(_tableView);
        layout->addSpacing(4);
        layout->addLayout(buttonLayout);

        connect(
            _tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &PositionSelectionDialog::_onSelectionChanged
        );
        connect(
            _selectBtn,
            &QPushButton::clicked,
            this,
            &PositionSelectionDialog::_onSelectClicked
        );
        connect(
            _createBtn,
            &QPushButton::clicked,
            this,
            &PositionSelectionDialog::_onCreateClicked
        );
        connect(_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    }

    /**
     * @brief Get the currently selected position draft
     *
     * @return std::optional<drafts::PositionStockDetailDraft>
     */
    std::optional<drafts::PositionStockDetailDraft> PositionSelectionDialog::
        selectedPosition() const
    {
        return _selectedPosition;
    }

    /**
     * @brief Slot called when the selection changes
     *
     */
    void PositionSelectionDialog::_onSelectionChanged()
    {
        const auto rows = _tableView->selectionModel()->selectedRows();
        _selectBtn->setEnabled(rows.size() == 1);
    }

    /**
     * @brief Slot called when the select button is clicked
     *
     */
    void PositionSelectionDialog::_onSelectClicked()
    {
        const auto rows = _tableView->selectionModel()->selectedRows();
        if (rows.size() != 1)
            return;

        _selectedPosition = _model->positionAt(rows.first().row());
        accept();
    }

    /**
     * @brief Slot called when the create button is clicked
     *
     */
    void PositionSelectionDialog::_onCreateClicked()
    {
        _selectedPosition = std::nullopt;
        accept();
    }
}   // namespace ui