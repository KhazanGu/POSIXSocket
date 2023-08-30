//
//  POSIXServer.swift
//  Server
//
//  Created by Khazan on 2023/8/30.
//

import Foundation

protocol POSIXServerDelegate {
    
    func didConnect(with client: POSIXClient) -> Void
    
}


class POSIXServer: NSObject {
    
    func run(on port: Int? = nil) -> Void {
        
        guard let socket = socket else { return }
        
        if let port = port {
            self.port = Int16(port)
        }
        
        server_socket_bind(socket, self.port)
        
        server_socket_listen_connections(socket)
        
        server_socket_accpet_connections_on_child_thread(socket) { client_socket in
            
            let client = POSIXClient(with: client_socket)
            
            self.delegate?.didConnect(with: client)
        }
        
    }
    
    override init() {
        super.init()
        
        self.socket = server_socket_create()
        
    }
    
    
    var socket: Int32?
    
    var port: Int16 = 8888
    
    var delegate: POSIXServerDelegate?
    
}
