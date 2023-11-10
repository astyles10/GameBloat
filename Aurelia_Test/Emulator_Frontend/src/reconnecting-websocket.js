// import { observable } 

// Probably need to make the constructor asynchronous in case of websocket hanging

export class ReconnectingWebsocket {
  constructor(address) {
    this.address = address
    this.connected = false
    this.callbacks = {
    }

    this.socket = new WebSocket(this.address)
    // this.connect()
    this.registerCallbacks()
    this.timer = null
  }

  registerCallbacks() {
    this.socket.onopen = this.handleConnection.bind(this)
    this.socket.onclose = this.handleDisconnect.bind(this)
    this.socket.onmessage = this.handleMessage.bind(this)
  }

  connect() {
    if (!this.connected) {
      this.socket = new WebSocket(this.address)
      this.registerCallbacks()
    }
  }

  handleConnection() {
    clearTimeout(this.timer)
    console.log('handleConnection called')
    this.connected = true
    if (this.callbacks.onOpen) {
      this.callbacks.onOpen('websocket connected!')
    }
  }

  handleDisconnect() {
    console.log('handleDisconnect called')
    this.connected = false
    this.timer = setTimeout(this.connect(), 2000)
  }

  handleMessage(message) {
    if (this.callbacks.onMessage) {
      this.callbacks.onMessage(message.data)
    }
  }

  setOnOpen(callback) {
    if (typeof (callback) === 'function') {
      this.callbacks.onOpen = callback
    }
  }

  setOnClose(callback) {
    if (typeof (callback) === 'function') {
      this.callbacks.onClose = callback
    }
  }

  setOnMessage(callback) {
    if (typeof (callback) === 'function') {
      this.callbacks.onMessage = callback
    }
  }
}