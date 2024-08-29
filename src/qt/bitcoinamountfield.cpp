// Copyright (c) 2011-2015 The Lambda Core developers
// Copyright (c) 2021 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/bitcoinamountfield.h>

#include <qt/bitcoinunits.h>
#include <qt/guiconstants.h>
#include <qt/guiutil.h>
#include <qt/qvaluecombobox.h>

#include <QAbstractSpinBox>
#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>

#include <cassert>

/**
 * QSpinBox that uses fixed-point numbers internally and uses our own
 * formatting/parsing functions.
 */
class AmountSpinBox : public QAbstractSpinBox {
    Q_OBJECT

public:
    explicit AmountSpinBox(QWidget *parent)
        : QAbstractSpinBox(parent), currentUnit(LambdaUnits::BCH),
          singleStep(100000 * SATOSHI) {
        setAlignment(Qt::AlignRight);

        connect(lineEdit(), &QLineEdit::textEdited, this,
                &AmountSpinBox::valueChanged);
    }

    QValidator::State validate(QString &text, int &pos [[maybe_unused]]) const override {
        if (text.isEmpty()) {
            return QValidator::Intermediate;
        }
        // All spaces are ignored.
        QString digits = LambdaUnits::removeSpaces(text);
        // Return Invalid for non-numeric characters - this will prevent them from being typed/pasted into the input field.
        // Note the input field is intended for unsigned amounts only (in MoneyRange),
        // so plus/minus signs are also rejected even though the amount parser can handle them.
        for (const QChar& chr : digits) {
            if ((chr < '0' || chr > '9') && chr != '.' && chr != ',') {
                return QValidator::Invalid;
            }
        }
        // Also return Invalid if the digits do not form a parseable number.
        // Otherwise return Intermediate so that fixup() is called on defocus.
        bool valid = false;
        parse(digits, &valid);
        return valid ? QValidator::Intermediate : QValidator::Invalid;
    }

    void fixup(QString &input) const override {
        bool valid = false;
        Amount val = parse(input, &valid);
        if (valid) {
            input = LambdaUnits::format(currentUnit, val, false,
                                         LambdaUnits::separatorAlways);
            lineEdit()->setText(input);
        }
    }

    Amount value(bool *valid_out = nullptr) const {
        return parse(text(), valid_out);
    }

    void setValue(const Amount value) {
        lineEdit()->setText(LambdaUnits::format(
            currentUnit, value, false, LambdaUnits::separatorAlways));
        Q_EMIT valueChanged();
    }

    void stepBy(int steps) override {
        bool valid = false;
        Amount val = value(&valid);
        val = val + steps * singleStep;
        val = qMin(qMax(val, Amount::zero()), MAX_MONEY);
        setValue(val);
    }

    void setDisplayUnit(int unit) {
        bool valid = false;
        Amount val(value(&valid));
        currentUnit = unit;

        if (valid) {
            setValue(val);
        } else {
            clear();
        }
    }

    void setSingleStep(const Amount step) { singleStep = step; }

    QSize minimumSizeHint() const override {
        if (cachedMinimumSizeHint.isEmpty()) {
            ensurePolished();

            const QFontMetrics fm(fontMetrics());
            int h = lineEdit()->minimumSizeHint().height();
            int w = GUIUtil::TextWidth(
                fm, LambdaUnits::format(LambdaUnits::BCH,
                                         MAX_MONEY, false,
                                         LambdaUnits::separatorAlways));
            // Cursor blinking space.
            w += 2;

            QStyleOptionSpinBox opt;
            initStyleOption(&opt);
            QSize hint(w, h);
            QSize extra(35, 6);
            opt.rect.setSize(hint + extra);
            extra +=
                hint - style()
                           ->subControlRect(QStyle::CC_SpinBox, &opt,
                                            QStyle::SC_SpinBoxEditField, this)
                           .size();
            // Get closer to final result by repeating the calculation.
            opt.rect.setSize(hint + extra);
            extra +=
                hint - style()
                           ->subControlRect(QStyle::CC_SpinBox, &opt,
                                            QStyle::SC_SpinBoxEditField, this)
                           .size();
            hint += extra;
            hint.setHeight(h);

            opt.rect = rect();

            cachedMinimumSizeHint =
                style()
                    ->sizeFromContents(QStyle::CT_SpinBox, &opt, hint, this)
                    .expandedTo(QApplication::globalStrut());
        }
        return cachedMinimumSizeHint;
    }

private:
    int currentUnit;
    Amount singleStep;
    mutable QSize cachedMinimumSizeHint;

    /**
     * Parse a string into a number of base monetary units and
     * return validity.
     * @note Must return 0 if !valid.
     */
    Amount parse(const QString &text, bool *valid_out = nullptr) const {
        auto val = LambdaUnits::parse(currentUnit, true, text);
        bool valid = val && MoneyRange(*val);
        if (valid_out) {
            *valid_out = valid;
        }
        return valid ? *val : Amount::zero();
    }

protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
            assert(keyEvent != nullptr);
            // Translate a comma into a period or vice versa, depending on locale.
            bool preferComma = LambdaUnits::decimalSeparatorIsComma();
            if (keyEvent->key() == (preferComma ? Qt::Key_Period : Qt::Key_Comma)) {
                QKeyEvent replacementKeyEvent(event->type(),
                                              preferComma ? Qt::Key_Comma : Qt::Key_Period,
                                              keyEvent->modifiers(),
                                              preferComma ? "," : ".",
                                              keyEvent->isAutoRepeat(), keyEvent->count());
                return QAbstractSpinBox::event(&replacementKeyEvent);
            }
        }
        return QAbstractSpinBox::event(event);
    }

    StepEnabled stepEnabled() const override {
        if (isReadOnly()) {
            // Disable steps when AmountSpinBox is read-only.
            return StepNone;
        }
        if (text().isEmpty()) {
            // Allow step-up with empty field.
            return StepUpEnabled;
        }

        StepEnabled rv = StepNone;
        bool valid = false;
        Amount val = value(&valid);
        if (valid) {
            if (val > Amount::zero()) {
                rv |= StepDownEnabled;
            }
            if (val < MAX_MONEY) {
                rv |= StepUpEnabled;
            }
        }
        return rv;
    }

Q_SIGNALS:
    void valueChanged();
};

#include <qt/bitcoinamountfield.moc>

LambdaAmountField::LambdaAmountField(QWidget *parent)
    : QWidget(parent), amount(nullptr) {
    amount = new AmountSpinBox(this);
    amount->setLocale(QLocale::c());
    amount->installEventFilter(this);
    amount->setMaximumWidth(240);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(amount);
    unit = new QValueComboBox(this);
    unit->setModel(new LambdaUnits(this));
    layout->addWidget(unit);
    layout->addStretch(1);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(amount);

    // If one if the widgets changes, the combined content changes as well
    connect(amount, &AmountSpinBox::valueChanged, this,
            &LambdaAmountField::valueChanged);
    connect(
        unit,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &LambdaAmountField::unitChanged);

    // Set default based on configuration
    unitChanged(unit->currentIndex());
}

void LambdaAmountField::clear() {
    amount->clear();
    unit->setCurrentIndex(0);
}

void LambdaAmountField::setEnabled(bool fEnabled) {
    amount->setEnabled(fEnabled);
    unit->setEnabled(fEnabled);
}

bool LambdaAmountField::validate() {
    bool valid = false;
    value(&valid);
    setValid(valid);
    return valid;
}

void LambdaAmountField::setValid(bool valid) {
    if (valid) {
        amount->setStyleSheet("");
    } else {
        amount->setStyleSheet(STYLE_INVALID);
    }
}

bool LambdaAmountField::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::FocusIn) {
        // Clear invalid flag on focus
        setValid(true);
    }
    return QWidget::eventFilter(object, event);
}

QWidget *LambdaAmountField::setupTabChain(QWidget *prev) {
    QWidget::setTabOrder(prev, amount);
    QWidget::setTabOrder(amount, unit);
    return unit;
}

Amount LambdaAmountField::value(bool *valid_out) const {
    return amount->value(valid_out);
}

void LambdaAmountField::setValue(const Amount value) {
    amount->setValue(value);
}

void LambdaAmountField::setReadOnly(bool fReadOnly) {
    amount->setReadOnly(fReadOnly);
}

void LambdaAmountField::unitChanged(int idx) {
    // Use description tooltip for current unit for the combobox
    unit->setToolTip(unit->itemData(idx, Qt::ToolTipRole).toString());

    // Determine new unit ID
    int newUnit = unit->itemData(idx, LambdaUnits::UnitRole).toInt();

    amount->setDisplayUnit(newUnit);
}

void LambdaAmountField::setDisplayUnit(int newUnit) {
    unit->setValue(newUnit);
}

void LambdaAmountField::setSingleStep(const Amount step) {
    amount->setSingleStep(step);
}
