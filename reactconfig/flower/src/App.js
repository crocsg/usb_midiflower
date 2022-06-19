import { BrowserRouter, Routes, Route } from 'react-router-dom';

import './App.css';
import Layout from "./Layout";
import NoPage from "./pages/NoPage";
import Home from "./pages/Home";
import Data from "./pages/Data";
import Channels  from './pages/Channels';

window.url_prefix = "";

const App = props => {
  
  return (
    
      <BrowserRouter>
          <Routes>
            <Route path="/" element={<Layout />}>
              <Route index element={<Home />} />
              
              <Route path="data" element={<Data />} />
              <Route path="channels" element={<Channels />} />
              
              <Route path="*" element={<NoPage />} />
            </Route>
          </Routes>
        </BrowserRouter>
    
  );
}

export default App;
