export class ReconnectingWebsocket {
  constructor(address) {
    this.address = address
    this.connected = false
    this.callbacks = {
      onOpen: null,
      onClose: null,
      onMessage: null
    }

    this.socket = new WebSocket(this.address)
  }

  connect() {
    this.socket = new WebSocket(this.address)
    if (this.socket.OPEN) {
      this.connected = true
      this.socket.onopen = this.handleConnection.bind(this)
      this.socket.onclose = this.handleDisconnect.bind(this)
      this.socket.onmessage = this.handleMessage.bind(this)
    }
  }

  handleConnection() {
    console.log('handleConnection called')
    if (this.callbacks.onOpen !== null) {
      this.callbacks.onOpen('websocket connected!')
    }
  }

  handleDisconnect() {
    console.log('handleDisconnect called')
  }

  handleMessage() {
    console.log('handleMessage called')
  }

  setOnOpen(callback) {
    if (typeof(callback) === 'function') {
      this.callbacks.onOpen = callback
    }
  }

  setOnClose(callback) {
    if (typeof(callback) === 'function') {
      this.callbacks.onClose = callback
    }
  }

  setOnMessage(callback) {
    if (typeof(callback) === 'function') {
      this.callbacks.onMessage = callback
    }
  }
}