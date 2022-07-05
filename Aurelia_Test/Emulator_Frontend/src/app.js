import { BindingEngine } from 'aurelia-binding'
import { ReconnectingWebsocket } from './reconnecting-websocket'

export class App {
  static inject = [BindingEngine]
  constructor(BindingEngine) {
    this.myCanvas = HTMLCanvasElement
    this.BindingEngine = BindingEngine
    this.message = 'Hello World!'
    this.messageData = 'Data go here'
    this.socket = new ReconnectingWebsocket('ws://localhost:8089')
    // Browsers contain their own native websocket code, do not need to import node Websocket module!

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
  }
}
