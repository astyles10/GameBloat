import { BindingEngine } from 'aurelia-binding'
import { ReconnectingWebsocket } from './reconnecting-websocket'
import { GameboyCanvasCustomElement } from './components/gameboy-canvas'
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
    this.canvas.redraw()
  }

  resizeCanvas() {
    this.canvas.resize()
  }

  reset() {
    this.canvas.reset()
  }

  sendMessage() {
    this.socket.socket.send('request')
  }
}
