import { BindingEngine } from 'aurelia-binding'

export class App {
  static inject = [BindingEngine]
  constructor(BindingEngine) {
    this.BindingEngine = BindingEngine
    this.message = 'Hello World!'
    // Browsers contain their own native websocket code, do not need to import node Websocket module!
    this.socket = new WebSocket('wss://demo.piesocket.com/v3/channel_1?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self')
    this.socket.onopen = () => { 
      console.log('Socket opened!')
    }
    this.socket.onmessage = (data) => {
      this.message = data
    }
  }

  bind () {
    console.log(this.socket)
  }
}
