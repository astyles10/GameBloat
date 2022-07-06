import { BindingEngine } from 'aurelia-binding'
import { ReconnectingWebsocket } from './reconnecting-websocket'
// import { io } from '../node_modules/socket.io-client/build/esm/index'

export class App {
  static inject = [BindingEngine]
  constructor(BindingEngine) {
    this.myCanvas = HTMLCanvasElement
    this.BindingEngine = BindingEngine
    this.message = 'Hello World!'
    this.messageData = 'Data go here'
    this.socket = new ReconnectingWebsocket('ws://localhost:8089')
    this.socket.setOnOpen((str) => {
      console.log(str)
    })
    this.socket.setOnMessage((messageData) => {
     this.messageData = messageData
    })
    // this.socket = io('ws://127.0.0.1:8089', {
    //   withCredentials: false
    // })
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
    this.socket.socket.send('request')
  }
}
