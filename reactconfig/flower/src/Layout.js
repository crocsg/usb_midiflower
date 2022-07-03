import { Outlet, Link} from "react-router-dom";
import { Flex, Box } from 'rebass'
import React from 'react'
import logo from './logo.png';
import './App.css';



const Layout = () => {
  return (
    <>
    <div className="App">
      <header className="App-header">
        <Flex>
          <Box p={3}>
        <img src={logo} className="App-logo" alt="logo" />
        </Box>
        <Box p={3}>
        <Link to="/" className="App-link">
            Home
        </Link>
        </Box>
        <Box p={3}>
        <Link to="/data" className="App-link">
            Sensor Data
        </Link>
        </Box>
        <Box p={3}>
        <Link to="/param" className="App-link">
            General Parameters
        </Link>
        </Box>
        <Box p={3}>
        <Link to="/channels" className="App-link">
            Channels Parameters
        </Link>
        </Box>
        </Flex>
        </header>
        <main>
          <Outlet/>
        </main>
    </div>
    
    </>
  )
}

export default Layout;