import { BrowserRouter, Routes, Route } from 'react-router-dom';

import './App.css';
import Layout from "./Layout";
import NoPage from "./pages/NoPage";
import Home from "./pages/Home";
import Data from "./pages/Data";
import Channels  from './pages/Channels';
import Parameters from './pages/Parameters'

//window.url_prefix = "http://192.168.7.1";
window.url_prefix = "";

const App = props => {
  
  return (
    
      <BrowserRouter>
          <Routes>
            <Route path="/" element={<Layout />}>
              <Route index element={<Home />} />
              
              <Route path="/data" element={<Data />} />
              <Route path="/param" element={<Parameters />} />
              <Route path="/channels" element={<Channels />} />
              
              <Route path="*" element={<Home />} />
            </Route>
          </Routes>
        </BrowserRouter>
    
  );
}

export default App;
