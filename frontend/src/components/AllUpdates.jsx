import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import AdminLoader from './AdminLoader';

const AllUpdates = () => {
  const [updates, setUpdates] = useState([]);
  const [loading, setLoading] = useState(true);
  const token = localStorage.getItem('token');

  const extractEventDate = (description) => {
    if (!description) return null;
    const match = String(description).match(/\bEvent Date:\s*(.+)\s*$/m);
    if (!match) return null;
    const value = match[1]?.trim();
    if (!value) return null;
    const parsed = new Date(value);
    return Number.isNaN(parsed.getTime()) ? null : parsed;
  };

  const stripEventDateLine = (description) => {
    if (!description) return "";
    return String(description)
      .replace(/\n\nEvent Date:.*$/ms, "")
      .trim();
  };

  useEffect(() => {
    // Fetch all updates when the component mounts
    const fetchUpdates = async () => {
      setLoading(true);
      try {
        const response = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/notifications`, {
          params: { type: "RECENT_UPDATES", page: 1, limit: 1000 },
        });
        setUpdates(response?.data?.data ?? []);
      } catch (error) {
        console.error('Error fetching updates:', error);
        toast.error('Failed to fetch updates. Please try again.');
      } finally {
        setLoading(false);
      }
    };
    fetchUpdates();
  }, []);

  // Handle delete update
  const handleDelete = async (id) => {
    if (!token) {
      toast.error('Please log in first.');
      return;
    }

    try {
      const response = await axios.delete(
        `${import.meta.env.VITE_API_URL}/api/v1/notifications/${id}`,
        {
          headers: { Authorization: `Bearer ${token}` },
        }
      );
      toast.success(response.data.message);
      // Remove the deleted update from the state
      setUpdates(updates.filter((update) => update.id !== id));
    } catch (error) {
      console.error('Error deleting update:', error);
      toast.error('Failed to delete update. Please try again.');
    }
  };

  return (
    <div className="space-y-6">
      <div>
        <h2 className="admin-title">All Updates</h2>
        <p className="admin-muted mt-1">Manage announcements and event updates.</p>
      </div>

      {loading ? (
        <div className="admin-card">
          <div className="admin-card-inner">
            <AdminLoader label="Loading updates..." />
          </div>
        </div>
      ) : updates.length === 0 ? (
        <p className="text-center text-zinc-600 text-sm">No updates found.</p>
      ) : (
        <div className="grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4">
          {updates.map((update) => (
            <div key={update.id} className="admin-card flex flex-col">
              <div className="admin-card-inner flex flex-col flex-1">
                <div className="flex-grow">
                <h3 className="text-base font-semibold text-zinc-900 mb-2 line-clamp-2">{update.title}</h3>
                <p className="text-zinc-700 text-sm mb-4 line-clamp-4">{stripEventDateLine(update.description).slice(0, 150)}...</p>
                {update.link && (
                  <a href={update.link} target="_blank" rel="noopener noreferrer" className="text-indigo-700 hover:text-indigo-800 text-sm mb-4 inline-block">
                    View Update Link
                  </a>
                )}
                <p className="text-zinc-600 text-sm mb-4">
                  Event Date: {(extractEventDate(update.description) || new Date(update.createdAt)).toLocaleString()}
                </p>
              </div>
              <div className="mt-auto">
                <button
                  onClick={() => handleDelete(update.id)}
                  className="admin-button-danger w-full"
                >
                  Delete
                </button>
              </div>
              </div>
            </div>
          ))}
        </div>
      )}
      <ToastContainer position="bottom-center" />
    </div>
  );
};

export default AllUpdates;
