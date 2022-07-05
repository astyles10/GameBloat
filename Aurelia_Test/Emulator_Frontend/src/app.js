import { BindingEngine } from 'aurelia-binding'

export class App {
  static inject = [BindingEngine]
  constructor(BindingEngine) {
    this.myCanvas = HTMLCanvasElement
    this.BindingEngine = BindingEngine
    this.message = 'Hello World!'
    this.messageData = 'Data go here'
    // Browsers contain their own native websocket code, do not need to import node Websocket module!
    this.socket = new WebSocket('ws://localhost:8089')
    this.socket.onopen = () => {
      console.log('Socket opened!')
    }
    this.socket.onmessage = (message) => {
      this.message = "Hello from: " + message.origin
      this.messageData = message.data
    }
  }

  bind () {
  }

  colorCanvas() {
    let thingy = this.myCanvas.getContext('2d')
    thingy.beginPath()
    thingy.rect(0, 0, 160, 144)
    let color = (thingy.fillStyle === '#ff0000' ? 'white' : 'red')
    thingy.fillStyle = color
    thingy.fill()
  }

  sendMessage() {
    this.socket.send('From Client')
  }
}
