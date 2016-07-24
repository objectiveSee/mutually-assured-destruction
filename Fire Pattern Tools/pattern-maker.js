var keyEvent = require('key-event')
var removeKeyEvent = keyEvent.off

keyEvent(element, 'ctrl alt u', function (event) {
  console.log('hi!')
})