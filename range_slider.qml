import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    RangeSlider {
        from: 1
        to: 100
        first.value: 25
        second.value: 75
    }
}
