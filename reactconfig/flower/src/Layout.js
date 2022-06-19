import { Outlet, Link} from "react-router-dom";
import React from 'react'
import logo from './cclab.png';
import './App.css';



const Layout = () => {
  return (
    <>
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        
        
        <Link to="/" className="App-link">
            Home
        </Link>
        <Link to="/data" className="App-link">
            Flower Data
        </Link>
        <Link to="/data" className="App-link">
            General parameters
        </Link>
        <Link to="/channels" className="App-link">
            Channels parameters
        </Link>
        
        </header>
        <main>
          <Outlet/>
        </main>
    </div>
    
    </>
  )
}

export default Layout;